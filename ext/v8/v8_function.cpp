#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_function.h"
#include "v8_external.h"
#include "v8_errors.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Function;
UNWRAPPER(Function);

/* Typecasting */

static Handle<Value> proc_caller(const Arguments &args)
{
  HandleScope scope;

  if (!args.Data().IsEmpty() && args.Data()->IsExternal()) {
    VALUE proc = (VALUE)External::Cast(*args.Data())->Value();
    VALUE proc_args[args.Length()];

    // We have to invoke `arity` on given proc instead of calling rb_proc_arity,
    // because we can have instance of Method instead of Proc...
    int proc_arity = FIX2INT(rb_funcall2(proc, rb_intern("arity"), 0, NULL));
    
    if (proc_arity < 0 || proc_arity == args.Length()) {
      for (int i = 0; i < args.Length(); i++) {
	proc_args[i] = to_ruby(args[i]);
      }

      // Again, we have to invoke `call` method like this instead of rb_proc_call...
      return to_v8(rb_funcall2(proc, rb_intern("call"), args.Length(), proc_args));
    } else {
      ThrowException(Exception::Error(String::New("wrong number of arguments")));
    }
  }

  return Null();
}

Handle<Value> to_v8_function(VALUE value)
{
  HandleScope scope;
  Handle<FunctionTemplate> func = FunctionTemplate::New(proc_caller, to_v8_external(value));
  return func->GetFunction();
}

/* V8::Function methods */

/*
 * call-seq:
 *   V8::Function.new(proc)    => new_func
 *   V8::Function.new { ... }  => new_func
 *
 * When block given then creates new function based on it, otherwise
 * creates function based on passed parameter (proc/lambda/method).
 *
 */
static VALUE rb_v8_function_new(int argc, VALUE *argv, VALUE klass)
{
  HandleScope scope;
  PREVENT_CREATION_WITHOUT_CONTEXT();

  VALUE orig;
  
  if (rb_block_given_p()) {
    orig = rb_block_proc();
  } else {
    if (argc == 1) {
      orig = argv[0];
    } else {
      rb_raise(rb_eArgError, "wrong number of arguments (%d for 1)", argc);
      return Qnil;
    }
  }

  VALUE self = v8_ref_new(klass, to_v8_function(orig), orig);
  rb_iv_set(self, "@origin", orig);
  rb_iv_set(self, "@receiver", Qnil);
  v8_set_peer(self);

  return self;
}

/*
 * call-seq:
 *   func.call_on(recv, *args)  => result
 *
 * Executes function with given arguments on specified receiver. When function code is
 * broken then proper JavaScript error will be returned. 
 *
 */
static VALUE rb_v8_function_call_on(int argc, VALUE *argv, VALUE self)
{
  HandleScope scope;
  TryCatch try_catch;

  if (argc < 1) {
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 1)", FIX2INT(argc));
    return Qnil;
  }
  
  VALUE recv = argv[0];
  
  Handle<Object> this_obj =
    NIL_P(recv) ? Context::GetEntered()->Global() :
    unwrap(recv)->ToObject();
  
  Handle<Value> args[argc-1];
  
  for (int i = 1; i < argc; i++) {
    args[i-1] = to_v8(argv[i]);
  }
  
  Handle<Value> result = unwrap(self)->Call(this_obj, argc-1, args);

  if (try_catch.HasCaught()) {
    return rb_v8_error_new3(try_catch);
  } else {
    return to_ruby(result);
  }
}

/*
 * call-seq:
 *   func.name  => str
 *
 * Returns function's internal name.
 *
 */
static VALUE rb_v8_function_get_name(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->GetName());
}

/*
 * call-seq:
 *   func.name = str  => str
 *
 * Sets function's internal name.
 *
 */
static VALUE rb_v8_function_set_name(VALUE self, VALUE name)
{
  HandleScope scope;
  unwrap(self)->SetName(String::New(StringValuePtr(name)));
  return name;
}

/*
 * call-seq:
 *   func.bind(obj)  => obj
 *
 * Binds given object as function's receiver. 
 *
 */
static VALUE rb_v8_function_bind(VALUE self, VALUE recv)
{
  return rb_iv_set(self, "@receiver", recv);
}


/* Public constructors */

VALUE rb_v8_function_new2(VALUE data)
{
  return rb_v8_function_new(1, &data, rb_cV8Function);
}


/* V8::Function initializer. */
void Init_V8_Function()
{
  rb_cV8Function = rb_define_class_under(rb_mV8, "Function", rb_cV8Object);
  rb_define_singleton_method(rb_cV8Function, "new", RUBY_METHOD_FUNC(rb_v8_function_new), -1);
  rb_define_method(rb_cV8Function, "bind", RUBY_METHOD_FUNC(rb_v8_function_bind), 1);
  rb_define_method(rb_cV8Function, "call_on", RUBY_METHOD_FUNC(rb_v8_function_call_on), -1);
  rb_define_method(rb_cV8Function, "name", RUBY_METHOD_FUNC(rb_v8_function_get_name), 0);
  rb_define_method(rb_cV8Function, "name=", RUBY_METHOD_FUNC(rb_v8_function_set_name), 1);
  rb_define_attr(rb_cV8Function, "receiver", 1, 0);
  rb_define_attr(rb_cV8Function, "origin", 1, 0);
}
