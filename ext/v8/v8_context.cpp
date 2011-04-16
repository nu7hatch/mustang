#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_context.h"
#include "v8_errors.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Context;
UNWRAPPER(Context);

/* V8::Context methods */

/*
 * call-seq:
 *   Mustang::V8::Context.current  => context
 *   Mustang::V8::Context.entered  => context
 *
 * Returns currently entered context.
 *
 */
static VALUE rb_v8_context_current(VALUE klass)
{
  if (Context::InContext()) {
    return rb_v8_wrapper_new(klass, Context::GetEntered());
  }
  
  return Qnil;
}

/*
 * call-seq:
 *   V8::Context.new  => new_context
 *
 * Returns new V8 context.
 *
 */
static VALUE rb_v8_context_new(VALUE klass)
{
  HandleScope scope;
  Persistent<Context> context(Context::New());
  VALUE self = rb_v8_context_new2(klass, context);  
  context.Dispose();
  return self;
}

/*
 * call-seq:
 *   cxt.enter                => true or false
 *   cxt.enter { |cxt| ... }  => nil
 *
 * Enters to context. Returns <code>true</code> when enter action has
 * been performed. If current context is already entered then returns
 * <code>false</code>.
 *
 * If block passed then context enters only for block execution, and
 * exits imidietely after that.
 *
 */
static VALUE rb_v8_context_enter(VALUE self)
{
  HandleScope scope;
  Handle<Context> context = unwrap(self);
  VALUE entered = Qfalse;

  if (Context::GetEntered() != context) {
    context->Enter();
    entered = Qtrue;
  }
  
  if (rb_block_given_p()) {
    rb_yield(self);
    context->Exit();
    return Qnil; 
  }
  
  return entered;
}

/*
 * call-seq:
 *   cxt.eval(source, filename)      => result
 *   cxt.evaluate(source, filename)  => result
 *
 * Evaluates given JavaScript code within current context.
 *
 *   cxt = V8::Context.new
 *   cxt.evaluate("1+1", "script.js")    # => 2
 *   cxt.evaluate("var a=1", "<eval>")   # => 1
 *   cxt.evaluate("var b=a+1", "<eval>") # => 2
 *
 */
static VALUE rb_v8_context_evaluate(VALUE self, VALUE source, VALUE filename)
{
  HandleScope scope;
  Local<String> _source(String::Cast(*to_v8(source)));
  Local<String> _filename(String::Cast(*to_v8(filename)));

  rb_v8_context_enter(self);

  TryCatch try_catch;
  Local<Script> script = Script::Compile(_source, _filename);

  if (!try_catch.HasCaught()) {
    Local<Value> result = script->Run();

    if (!try_catch.HasCaught()) {
      return to_ruby(result);
    }
  }

  return rb_v8_error_new3(try_catch);
}

/*
 * call-seq:
 *   cxt.prototype  => obj
 *
 * Returns prototype object of current context. 
 *
 */
VALUE rb_v8_context_prototype(VALUE self)
{
  HandleScope scope;
  Handle<Object> proto(Object::Cast(*unwrap(self)->Global()->GetPrototype()));
  return to_ruby(proto);
}

/*
 * call-seq:
 *   cxt.global  => value
 *
 * Returns global object for this context.
 *
 */
static VALUE rb_v8_context_global(VALUE self)
{
  return to_ruby(unwrap(self)->Global());
}

/*
 * call-seq:
 *   cxt.entered?  => true or false
 *
 * Returns <code>true</code> when this context is entered.
 *
 */
static VALUE rb_v8_context_entered_p(VALUE self)
{
  return unwrap(self) == Context::GetEntered() ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   cxt.exit  => nil
 *
 * Exits from context.
 *
 */
static VALUE rb_v8_context_exit(VALUE self)
{
  if (Context::InContext()) {
    unwrap(self)->Exit();
  }
  
  return Qnil;
}

/*
 * call-seq:
 *   V8::Context.exit_all!  => nil
 *
 * Exits from all entered context.
 *
 */
static VALUE rb_v8_context_exit_all_bang(VALUE klass)
{
  while (Context::InContext()) {
    Context::GetEntered()->Exit();
  }

  return Qnil;
}

/*
 * call-seq:
 *   cxt == other_cxt  => true or false
 *
 * Returns <code>true</code> when v8 context values are equals.
 *
 */
static VALUE rb_v8_context_equals_p(VALUE self, VALUE other)
{
  if (rb_obj_is_kind_of(other, rb_cV8Context) && unwrap(other) == unwrap(self)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

/* Public constructors. */

VALUE rb_v8_context_new2(VALUE klass, Handle<Context> context)
{
  VALUE ref = rb_v8_wrapper_new(klass, context);
  rb_v8_context_enter(ref);
  rb_iv_set(ref, "@errors", rb_ary_new());
  return ref;
}


/* V8::Context class initializer. */
void Init_V8_Context()
{
  rb_cV8Context = rb_define_class_under(rb_mV8, "Context", rb_cObject);
  rb_define_singleton_method(rb_cV8Context, "new", RUBY_METHOD_FUNC(rb_v8_context_new), 0);
  rb_define_singleton_method(rb_cV8Context, "exit_all!", RUBY_METHOD_FUNC(rb_v8_context_exit_all_bang), 0);
  rb_define_singleton_method(rb_cV8Context, "current", RUBY_METHOD_FUNC(rb_v8_context_current), 0);
  rb_define_singleton_method(rb_cV8Context, "entered", RUBY_METHOD_FUNC(rb_v8_context_current), 0);
  rb_define_method(rb_cV8Context, "==", RUBY_METHOD_FUNC(rb_v8_context_equals_p), 1);
  rb_define_method(rb_cV8Context, "equals?", RUBY_METHOD_FUNC(rb_v8_context_equals_p), 1);
  rb_define_method(rb_cV8Context, "evaluate", RUBY_METHOD_FUNC(rb_v8_context_evaluate), 2);
  rb_define_method(rb_cV8Context, "eval", RUBY_METHOD_FUNC(rb_v8_context_evaluate), 2);
  rb_define_method(rb_cV8Context, "prototype", RUBY_METHOD_FUNC(rb_v8_context_prototype), 0);
  rb_define_method(rb_cV8Context, "global", RUBY_METHOD_FUNC(rb_v8_context_global), 0);
  rb_define_method(rb_cV8Context, "enter", RUBY_METHOD_FUNC(rb_v8_context_enter), 0);
  rb_define_method(rb_cV8Context, "exit", RUBY_METHOD_FUNC(rb_v8_context_exit), 0);
  rb_define_method(rb_cV8Context, "entered?", RUBY_METHOD_FUNC(rb_v8_context_entered_p), 0);
  rb_define_attr(rb_cV8Context, "errors", 1, 0);
}
