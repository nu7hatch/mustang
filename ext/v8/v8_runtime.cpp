#include "v8_runtime.h"
#include "v8_reflections.h"

using namespace v8;
using namespace std;

static VALUE rb_cMustangV8Runtime;
static VALUE runtimeGetErrors(VALUE self);

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

/* Reports exceptions catched by V8 during code evaluation. This only gets all info
 * about encountered problem, rest of job is done by ruby's Mustang::JsErrors class. */
static void runtimeReportException(VALUE self, TryCatch *try_catch)
{
  HandleScope scope;
  String::Utf8Value exception(try_catch->Exception());
  Handle<Message> message = try_catch->Message();

  VALUE errinfo = rb_hash_new();
  rb_hash_aset(errinfo, rb_str_new2("message"), v82rbstr(exception));

  if (!message.IsEmpty()) {
    String::Utf8Value filename(message->GetScriptResourceName());
    String::Utf8Value sourceline(message->GetSourceLine());
 
    int linenum = message->GetLineNumber();
    int startcol = message->GetStartColumn();
    int endcol = message->GetEndColumn();

    rb_hash_aset(errinfo, rb_str_new2("file"), v82rbstr(filename));
    rb_hash_aset(errinfo, rb_str_new2("snippet"), v82rbstr(sourceline));
    rb_hash_aset(errinfo, rb_str_new2("line"), INT2FIX(linenum));

    VALUE pos = rb_ary_new();
    rb_ary_push(pos, INT2FIX(startcol));
    rb_ary_push(pos, INT2FIX(endcol));

    rb_hash_aset(errinfo, rb_str_new2("pos"), pos);
  }

  // Lets play with this on the ruby side...
  rb_funcall(runtimeGetErrors(self), rb_intern("push"), 1, errinfo);
}

/* Runtime instance methods */

/* Evaluates given source code. */
static VALUE runtimeEvaluate(VALUE self, VALUE source, VALUE filename)
{
  runtimeDefineScopes();
  
  Local<String> v8source = rbstr2v8(source);
  Local<String> v8filename = NIL_P(filename) ? cstr2v8("unknown") : rbstr2v8(filename);

  TryCatch try_catch;
  Local<Script> script = Script::Compile(v8source, v8filename);
  Local<Value> result(script->Run());

  if (result.IsEmpty()) {
    if (try_catch.HasCaught()) {
      runtimeReportException(self, &try_catch);
    }

    return Qnil;
  } else {
    String::Utf8Value str(result);
    return rb_str_new2(v82cstr(str));
  }
}

/* Get property value from the global object. */
static VALUE runtimeGetVar(VALUE self, VALUE name)
{
  runtimeDefineScopes();
  Handle<Value> value = runtimeGetGlobal(self)->Get(rb2handle(name));
  return handle2rb(value);
}

/* Set given property to the global object. */
static VALUE runtimeSetVar(VALUE self, VALUE name, VALUE value)
{
  runtimeDefineScopes();
  runtimeGetGlobal(self)->Set(rb2handle(name), rb2handle(value));
  return value;
}

/* Returns list of Javascript errors encountered during code evaluation. */
static VALUE runtimeGetErrors(VALUE self)
{
  VALUE errors = rb_iv_get(self, "@errors");

  if (NIL_P(errors)) {
    errors = rb_iv_set(self, "@errors", rb_ary_new());
  }

  return errors;
}

/* Mustang::V8::Runtime initializer. */
void initRuntime(VALUE parent) { 
  rb_cMustangV8Runtime = rb_define_class_under(parent, "Runtime", rb_cObject);

  rb_define_alloc_func(rb_cMustangV8Runtime, runtimeAllocate);
  rb_define_method(rb_cMustangV8Runtime, "evaluate", RUBY_METHOD_FUNC(runtimeEvaluate), 2);
  rb_define_method(rb_cMustangV8Runtime, "[]", RUBY_METHOD_FUNC(runtimeGetVar), 1);
  rb_define_method(rb_cMustangV8Runtime, "[]=", RUBY_METHOD_FUNC(runtimeSetVar), 2);
  rb_define_method(rb_cMustangV8Runtime, "errors", RUBY_METHOD_FUNC(runtimeGetErrors), 0);
}
