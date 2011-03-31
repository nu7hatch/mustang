#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Value;
UNWRAPPER(Value);

/* V8::Value methods */

/*
 * call-seq:
 *   V8::Value.new(obj)  => new_value
 *
 * Returns new V8 value reflected from given object.
 *
 */
static VALUE rb_v8_value_new(VALUE self, VALUE data)
{
  HandleScope scope;
  return v8_ref_new(self, to_v8(data));
}

static VALUE rb_v8_value_object_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsObject());
}

static VALUE rb_v8_value_integer_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsInt32() || unwrap(self)->IsUint32());
}

static VALUE rb_v8_value_number_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsNumber());
}

static VALUE rb_v8_value_string_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsString());
}

static VALUE rb_v8_value_external_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsExternal());
}

static VALUE rb_v8_value_function_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsFunction());
}

static VALUE rb_v8_value_array_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsArray());
}

static VALUE rb_v8_value_boolean_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsBoolean());
}

static VALUE rb_v8_value_true_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsTrue());
}

static VALUE rb_v8_value_false_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsFalse());
}

static VALUE rb_v8_value_empty_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self).IsEmpty());
}

static VALUE rb_v8_value_null_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsNull());
}

static VALUE rb_v8_value_undefined_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsUndefined());
}

static VALUE rb_v8_value_to_string(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToString());
}

static VALUE rb_v8_value_to_integer(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToInteger());
}

static VALUE rb_v8_value_to_number(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToNumber());
}

static VALUE rb_v8_value_to_object(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToObject());
}

/*
 * call-seq:
 *   val === other_val  => true or false
 *
 * Returns <code>true</code> when compared values are the same objects.
 *
 */
static VALUE rb_v8_value_equals(VALUE self, VALUE value)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->Equals(to_v8(value)));
}

/*
 * call-seq:
 *   val == other_val  => true or false
 *
 * Returns <code>true</code> when compared values are equal.
 *
 */
static VALUE rb_v8_value_strict_equals(VALUE self, VALUE value)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->StrictEquals(to_v8(value)));
}

/* Public constructors */

VALUE rb_v8_value_new2(VALUE data)
{
  return rb_v8_value_new(rb_cV8Value, data);
}


/* V8::Value class initializer. */
void Init_V8_Value()
{
  rb_cV8Value = rb_define_class_under(rb_mV8, "Value", rb_cV8Data);
  rb_define_singleton_method(rb_cV8Value, "new", RUBY_METHOD_FUNC(rb_v8_value_new), 1);
  rb_define_method(rb_cV8Value, "object?", RUBY_METHOD_FUNC(rb_v8_value_object_p), 0);
  rb_define_method(rb_cV8Value, "integer?", RUBY_METHOD_FUNC(rb_v8_value_integer_p), 0);
  rb_define_method(rb_cV8Value, "string?", RUBY_METHOD_FUNC(rb_v8_value_string_p), 0);
  rb_define_method(rb_cV8Value, "number?", RUBY_METHOD_FUNC(rb_v8_value_number_p), 0);
  rb_define_method(rb_cV8Value, "external?", RUBY_METHOD_FUNC(rb_v8_value_external_p), 0);
  rb_define_method(rb_cV8Value, "array?", RUBY_METHOD_FUNC(rb_v8_value_array_p), 0);
  rb_define_method(rb_cV8Value, "function?", RUBY_METHOD_FUNC(rb_v8_value_function_p), 0);
  rb_define_method(rb_cV8Value, "boolean?", RUBY_METHOD_FUNC(rb_v8_value_boolean_p), 0);
  rb_define_method(rb_cV8Value, "true?", RUBY_METHOD_FUNC(rb_v8_value_true_p), 0);
  rb_define_method(rb_cV8Value, "false?", RUBY_METHOD_FUNC(rb_v8_value_false_p), 0);
  rb_define_method(rb_cV8Value, "empty?", RUBY_METHOD_FUNC(rb_v8_value_empty_p), 0);
  rb_define_method(rb_cV8Value, "undefined?", RUBY_METHOD_FUNC(rb_v8_value_undefined_p), 0);
  rb_define_method(rb_cV8Value, "null?", RUBY_METHOD_FUNC(rb_v8_value_null_p), 0);
  rb_define_method(rb_cV8Value, "to_string", RUBY_METHOD_FUNC(rb_v8_value_to_string), 0);
  rb_define_method(rb_cV8Value, "to_integer", RUBY_METHOD_FUNC(rb_v8_value_to_integer), 0);
  rb_define_method(rb_cV8Value, "to_number", RUBY_METHOD_FUNC(rb_v8_value_to_number), 0);
  rb_define_method(rb_cV8Value, "to_object", RUBY_METHOD_FUNC(rb_v8_value_to_object), 0);
  rb_define_method(rb_cV8Value, "===", RUBY_METHOD_FUNC(rb_v8_value_strict_equals), 1);
  rb_define_method(rb_cV8Value, "==", RUBY_METHOD_FUNC(rb_v8_value_equals), 1);
}
