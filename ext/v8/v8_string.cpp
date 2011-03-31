#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_string.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8String;
UNWRAPPER(String);

/* Typecasting helpers */

Handle<Value> to_v8_string(VALUE value)
{
  return String::New(StringValuePtr(value));
}

/* V8::String methods */

/*
 * call-seq:
 *   V8::String.new(str)  => new_string
 *
 * Returns new V8 string reflected from given string.
 *
 */
static VALUE rb_v8_string_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  VALUE str = rb_funcall2(data, rb_intern("to_s"), 0, NULL);
  return v8_ref_new(klass, to_v8_string(str));
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
  return rb_str_new2(*String::Utf8Value(unwrap(self)));
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

/* Public constructors */

VALUE rb_v8_string_new2(VALUE data)
{
  return rb_v8_string_new(rb_cV8String, data);
}


/* V8::String initializer. */
void Init_V8_String()
{
  rb_cV8String = rb_define_class_under(rb_mV8, "String", rb_cV8Primitive);
  rb_define_singleton_method(rb_cV8String, "new", RUBY_METHOD_FUNC(rb_v8_string_new), 1);
  rb_define_method(rb_cV8String, "to_ascii", RUBY_METHOD_FUNC(rb_v8_string_to_ascii), 0);
  rb_define_method(rb_cV8String, "to_utf8", RUBY_METHOD_FUNC(rb_v8_string_to_utf8), 0);
  rb_define_method(rb_cV8String, "to_s", RUBY_METHOD_FUNC(rb_v8_string_to_utf8), 0);
}
