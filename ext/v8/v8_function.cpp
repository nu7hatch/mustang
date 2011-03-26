#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_function.h"

using namespace v8;

VALUE rb_cMustangV8Function;

/* Typecasting */

VALUE v8_function_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<Function> func(Function::Cast(*value));
  return v8_ref_new(rb_cMustangV8Function, func);
}

Handle<Value> v8_function_cast(VALUE value)
{
  //HandleScope scope;
  //return scope.Close(Function::New());
}

/* Local heleprs */

static Local<Function> unwrap(VALUE self)
{
  return v8_ref_get<Function>(self);
}  

/* Mustang::V8::Function methods */

//static VALUE rb_v8_string_new(VALUE klass, VALUE data)
//{
//  HandleScope scope;
//  Handle<Function> func = Function::New()
//  VALUE str = rb_funcall(data, rb_intern("to_s"), 0);
//  return v8_ref_new(klass, String::New(StringValuePtr(str)));
//}

static VALUE rb_v8_function_call(int argc, VALUE *args, VALUE self)
{
  HandleScope scope;
  Local<Object> this_obj;
  Handle<Value> fargs[argc];

  VALUE _this_obj = rb_iv_get(self, "@this");

  if (NIL_P(_this_obj)) {
    this_obj = unwrap(self);
  } else {
    this_obj = v8_ref_get<Value>(_this_obj)->ToObject();
  }
  
  for (int i = 0; i < argc; i++) {
    fargs[i] = to_v8(args[i]);
  }
  
  return to_ruby(unwrap(self)->Call(this_obj, argc, fargs));
}


/* Mustang::V8::Function initializer. */
void Init_V8_Function()
{
  rb_cMustangV8Function = rb_define_class_under(rb_mMustangV8, "Function", rb_cMustangV8Object);
  rb_define_attr(rb_cMustangV8Function, "this", 1, 1);
  rb_define_method(rb_cMustangV8Function, "call", RUBY_METHOD_FUNC(rb_v8_function_call), -1);
}
