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
    VALUE proc_args = rb_ary_new();
    int proc_arity = rb_proc_arity(proc);
    
    if (proc_arity < 0 || proc_arity == args.Length()) {
      for (int i = 0; i < args.Length(); i++) {
	rb_ary_push(proc_args, to_ruby(args[i]));
      }
   
      return to_v8(rb_proc_call(proc, proc_args));
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

static VALUE rb_v8_function_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  return v8_ref_new(klass, to_v8_function(data));
}

static VALUE rb_v8_function_call(int argc, VALUE *args, VALUE self)
{
  HandleScope scope;
  TryCatch try_catch;
  
  VALUE recv = rb_iv_get(self, "@receiver");

  Handle<Object> this_obj =
    NIL_P(recv) ? Context::GetEntered()->Global() :
    unwrap(recv)->ToObject();
  
  Handle<Value> fargs[argc];
  
  for (int i = 0; i < argc; i++) {
    fargs[i] = to_v8(args[i]);
  }
  
  Handle<Value> result = unwrap(self)->Call(this_obj, argc, fargs);

  if (try_catch.HasCaught()) {
    return rb_v8_error_new3(try_catch);
  } else {
    return to_ruby(result);
  }
}

static VALUE rb_v8_function_get_name(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->GetName());
}

static VALUE rb_v8_function_set_name(VALUE self, VALUE name)
{
  HandleScope scope;
  unwrap(self)->SetName(String::New(StringValuePtr(name)));
  return name;
}

static VALUE rb_v8_function_bind(VALUE self, VALUE recv)
{
  return rb_iv_set(self, "@receiver", recv);
}

/* Public constructors */

VALUE rb_v8_function_new2(VALUE data)
{
  return rb_v8_function_new(rb_cV8Function, data);
}


/* V8::Function initializer. */
void Init_V8_Function()
{
  rb_cV8Function = rb_define_class_under(rb_mV8, "Function", rb_cV8Object);
  rb_define_singleton_method(rb_cV8Function, "new", RUBY_METHOD_FUNC(rb_v8_function_new), 1);
  rb_define_method(rb_cV8Function, "bind", RUBY_METHOD_FUNC(rb_v8_function_bind), 1);
  rb_define_method(rb_cV8Function, "call", RUBY_METHOD_FUNC(rb_v8_function_call), -1);
  rb_define_method(rb_cV8Function, "name", RUBY_METHOD_FUNC(rb_v8_function_get_name), 0);
  rb_define_method(rb_cV8Function, "name=", RUBY_METHOD_FUNC(rb_v8_function_set_name), 1);
  rb_define_attr(rb_cV8Function, "receiver", 1, 0);
}
