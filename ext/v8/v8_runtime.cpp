#include "v8_runtime.h"
#include "v8_reflections.h"

using namespace v8;
using namespace std;

static VALUE rb_cMustangV8Runtime;

/* Garbage collector interaction for Runtime */

static VALUE runtimeMark(struct runtime *r)
{
  return Qnil;
}

static VALUE runtimeFree(struct runtime *r)
{
  r->context.Dispose();
  return Qnil;
}

static VALUE runtimeAllocate(VALUE klass)
{
  HandleScope scope;
  Handle<ObjectTemplate> global_template = Handle<ObjectTemplate>();
  Handle<Object> global_object = Handle<Object>();

  struct runtime *r = new runtime();
  r->context = Context::New(NULL, global_template, global_object);

  return Data_Wrap_Struct(klass, runtimeMark, runtimeFree, r);
}

/* Internal helpers */

/* Returns global execution context. */
static Local<Context> runtimeGetContext(VALUE self)
{
  struct runtime *r = 0;
  Data_Get_Struct(self, struct runtime, r);
  return (Context*)*r->context;
}

/* Returns global object. */
static Handle<Object> runtimeGetGlobal(VALUE self)
{
  HandleScope scope;
  return Handle<Object>::Cast(runtimeGetContext(self)->Global()->GetPrototype());
}

/* Runtime instance methods */

/* Evaluates given source code. */
static VALUE runtimeEvaluate(VALUE self, VALUE source, VALUE filename)
{
  runtimeDefineScopes();
  
  Local<String> v8source = rbstr2v8(source);
  Local<String> v8filename = NIL_P(filename) ? cstr2v8("unknown") : rbstr2v8(filename);

  Local<Script> script = Script::Compile(v8source, v8filename);
  Local<Value> result(script->Run());

  if (result.IsEmpty()) {
    return Qnil;
  } else {
    String::Utf8Value resultstr(String::Cast(*result)->ToString());
    return rb_str_new2(string(*resultstr).c_str());
  }
}

/* Get property value from the global object. */
static VALUE runtimeGetVar(VALUE self, VALUE name)
{
  runtimeDefineScopes();
  Handle<Value> value = runtimeGetGlobal(self)->Get(rbstr2v8(name));
  return handle2rb(value);
}

/* Set given property to the global object. */
static VALUE runtimeSetVar(VALUE self, VALUE name, VALUE value)
{
  runtimeDefineScopes();
  runtimeGetGlobal(self)->Set(rbstr2v8(name), rb2handle(value));
  return value;
}

/* Mustang::V8::Runtime initializer. */
void initRuntime(VALUE parent) { 
  rb_cMustangV8Runtime = rb_define_class_under(parent, "Runtime", rb_cObject);

  rb_define_alloc_func(rb_cMustangV8Runtime, runtimeAllocate);
  rb_define_method(rb_cMustangV8Runtime, "evaluate", RUBY_METHOD_FUNC(runtimeEvaluate), 2);
  rb_define_method(rb_cMustangV8Runtime, "[]", RUBY_METHOD_FUNC(runtimeGetVar), 1);
  rb_define_method(rb_cMustangV8Runtime, "[]=", RUBY_METHOD_FUNC(runtimeSetVar), 2);
}
