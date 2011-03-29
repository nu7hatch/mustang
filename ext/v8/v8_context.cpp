#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_context.h"
#include "v8_macros.h"
#include "v8_exceptions.h"

using namespace v8;

VALUE rb_cV8Context;
UNWRAPPER(Context);

/* Local helpers */

Handle<Object> v8_context_get_prototype(VALUE self)
{
  HandleScope scope;
  return Handle<Object>::Cast(unwrap(self)->Global()->GetPrototype());
}

/* V8::Context methods */

/*
 * call-seq:
 *   V8::Context.new  => new_context
 *
 * Returns new V8 context.
 *
 */
static VALUE rb_v8_context_new(VALUE self)
{
  HandleScope scope;
  Persistent<Context> context(Context::New());

  VALUE ref = v8_ref_new(self, context);
  rb_iv_set(ref, "@errors", rb_ary_new());

  context.Dispose();
  return ref;
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

  VALUE exception = rb_v8_exception_new2(&try_catch);
  rb_ary_push(rb_iv_get(self, "@errors"), exception);

  //if (rb_iv_get("@debug") == Qtrue) {
  //  // TODO: Change to proper error
  //  rb_raise(rb_eRuntimeError, rb_funcall2(exception, rb_intern("message"), 0));
  //}

  return Qnil;
}

/*
 * call-seq:
 *   cxt[key]      => value
 *   cxt.get(key)  => value
 *
 * Get given property from the global object proto.
 *
 *   cxt = V8::Context.new
 *   cxt.eval("var foo = 'bar'")
 *   cxt["foo"] # => 'bar'
 *
 */
static VALUE rb_v8_context_get(VALUE self, VALUE key)
{
  HandleScope scope;
  return to_ruby(v8_context_get_prototype(self)->Get(to_v8(key)));
}

/*
 * call-seq:
 *   cxt[key] = value     => value
 *   cxt.set(key, value)  => value
 *
 * Set given property within the global object proto.
 *
 *   cxt = V8::Context.new
 *   cxt["foo"] = "bar"
 *   cxt.eval("foo") # => 'bar'
 *
 */
static VALUE rb_v8_context_set(VALUE self, VALUE key, VALUE value)
{
  HandleScope scope;
  Handle<Value> _value = to_v8(value);
  v8_context_get_prototype(self)->Set(to_v8(key), _value);
  return to_ruby(_value);
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
  HandleScope scope;
  Handle<Value> global = unwrap(self)->Global();
  return to_ruby(global);
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
  HandleScope scope;
  unwrap(self)->Exit();
  return Qnil;
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
  HandleScope scope;
  return unwrap(self) == Context::GetEntered() ? Qtrue : Qfalse;
}


/* V8::Context class initializer. */
void Init_V8_Context()
{
  rb_cV8Context = rb_define_class_under(rb_mV8, "Context", rb_cObject);
  rb_define_singleton_method(rb_cV8Context, "new", RUBY_METHOD_FUNC(rb_v8_context_new), 0);
  rb_define_method(rb_cV8Context, "evaluate", RUBY_METHOD_FUNC(rb_v8_context_evaluate), 2);
  rb_define_method(rb_cV8Context, "eval", RUBY_METHOD_FUNC(rb_v8_context_evaluate), 2);
  rb_define_method(rb_cV8Context, "[]", RUBY_METHOD_FUNC(rb_v8_context_get), 1);
  rb_define_method(rb_cV8Context, "get", RUBY_METHOD_FUNC(rb_v8_context_get), 1);
  rb_define_method(rb_cV8Context, "[]=", RUBY_METHOD_FUNC(rb_v8_context_set), 2);
  rb_define_method(rb_cV8Context, "set", RUBY_METHOD_FUNC(rb_v8_context_set), 2);
  rb_define_method(rb_cV8Context, "global", RUBY_METHOD_FUNC(rb_v8_context_global), 0);
  rb_define_method(rb_cV8Context, "enter", RUBY_METHOD_FUNC(rb_v8_context_enter), 0);
  rb_define_method(rb_cV8Context, "exit", RUBY_METHOD_FUNC(rb_v8_context_exit), 0);
  rb_define_method(rb_cV8Context, "entered?", RUBY_METHOD_FUNC(rb_v8_context_entered_p), 0);
  rb_define_attr(rb_cV8Context, "errors", 1, 0);
}
