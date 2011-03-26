#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"

using namespace v8;

VALUE rb_cMustangV8String;

/* Typecasting helpers */

VALUE v8_string_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<String> str(String::Cast(*value));
  return v8_ref_new(rb_cMustangV8String, str);
}

Handle<Value> v8_string_cast(VALUE value)
{
  HandleScope scope;
  return scope.Close(String::New(StringValuePtr(value)));
}

/* Local helpers */

static Local<String> unwrap(VALUE self)
{
  return v8_ref_get<String>(self);
}  

/* Mustang::V8::String methods */

static VALUE rb_v8_string_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  VALUE str = rb_funcall2(data, rb_intern("to_s"), 0, NULL);
  return v8_ref_new(klass, v8_string_cast(str));
}

/*
 * call-seq:
 *   str.to_utf8  => value
 *   str.to_s     => value
 *
 * Returns UTF-8 representation of referenced string.
 *
 */
static VALUE rb_v8_string_to_utf8(VALUE self)
{
  HandleScope scope;
  VALUE str = rb_str_new2(*String::Utf8Value(unwrap(self)));
  return rb_funcall(str, rb_intern("force_encoding"), 1, rb_str_new2("UTF-8"));
}

/*
 * call-seq:
 *   str.to_ascii  => value
 *
 * Returns ASCII representation of referenced string.
 *
 */
static VALUE rb_v8_string_to_ascii(VALUE self)
{
  HandleScope scope;
  return rb_str_new2(*String::AsciiValue(unwrap(self)));
}


/* Mustang::V8::String initializer. */
void Init_V8_String()
{
  rb_cMustangV8String = rb_define_class_under(rb_mMustangV8, "String", rb_cMustangV8Object);
  rb_define_singleton_method(rb_cMustangV8String, "new", RUBY_METHOD_FUNC(rb_v8_string_new), 1);
  rb_define_method(rb_cMustangV8String, "to_ascii", RUBY_METHOD_FUNC(rb_v8_string_to_ascii), 0);
  rb_define_method(rb_cMustangV8String, "to_utf8", RUBY_METHOD_FUNC(rb_v8_string_to_utf8), 0);
  rb_define_method(rb_cMustangV8String, "to_s", RUBY_METHOD_FUNC(rb_v8_string_to_utf8), 0);
}
