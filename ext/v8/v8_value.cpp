#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_base.h"
#include "v8_value.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Value;
VALUE rb_cV8Primitive;
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
  PREVENT_CREATION_WITHOUT_CONTEXT();
  return v8_ref_new(self, to_v8(data));
}

/*
 * call-seq:
 *   val.object?  => true or false
 *   val.obj?     => true or false
 *
 * Returns <code>true</code> when value is a javascipt object.
 *
 */
static VALUE rb_v8_value_object_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsObject());
}

/*
 * call-seq:
 *   val.integer?  => true or false
 *   val.int?      => true or false
 *
 * Returns <code>true</code> when value is a javascipt integer.
 *
 */
static VALUE rb_v8_value_integer_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsInt32() || unwrap(self)->IsUint32());
}

/*
 * call-seq:
 *   val.number?  => true or false
 *   val.num?     => true or false
 *
 * Returns <code>true</code> when value is a javascipt floating point number.
 *
 */
static VALUE rb_v8_value_number_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsNumber());
}

/*
 * call-seq:
 *   val.string?  => true or false
 *   val.str?     => true or false
 *
 * Returns <code>true</code> when value is a javascipt string.
 *
 */
static VALUE rb_v8_value_string_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsString());
}

/*
 * call-seq:
 *   val.external?  => true or false
 *
 * Returns <code>true</code> when value is a v8 external value.
 *
 */
static VALUE rb_v8_value_external_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsExternal());
}

/*
 * call-seq:
 *   val.function?  => true or false
 *   val.func?      => true or false
 *
 * Returns <code>true</code> when value is a javascipt function.
 *
 */
static VALUE rb_v8_value_function_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsFunction());
}

/*
 * call-seq:
 *   val.array?  => true or false
 *   val.ary?    => true or false
 *
 * Returns <code>true</code> when value is a javascipt array.
 *
 */
static VALUE rb_v8_value_array_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsArray());
}

/*
 * call-seq:
 *   val.boolean?  => true or false
 *   val.bool?     => true or false
 *
 * Returns <code>true</code> when value is a javascipt boolean.
 *
 */
static VALUE rb_v8_value_boolean_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsBoolean());
}

/*
 * call-seq:
 *   val.regexp?  => true or false
 *   val.regex?   => true or false
 *
 * Returns <code>true</code> when value is a javascipt regexp.
 *
 */
static VALUE rb_v8_value_regexp_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsRegExp());
}

/*
 * call-seq:
 *   val.date?  => true or false
 *
 * Returns <code>true</code> when value is a javascipt date.
 *
 */
static VALUE rb_v8_value_date_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsDate());
}

/*
 * call-seq:
 *   val.true?  => true or false
 *
 * Returns <code>true</code> when value is a javascipt bool true.
 *
 */
static VALUE rb_v8_value_true_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsTrue());
}

/*
 * call-seq:
 *   val.false?  => true or false
 *
 * Returns <code>true</code> when value is a javascipt bool false.
 *
 */
static VALUE rb_v8_value_false_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsFalse());
}

/*
 * call-seq:
 *   val.empty?  => true or false
 *
 * Returns <code>true</code> when value is empty.
 *
 */
static VALUE rb_v8_value_empty_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self).IsEmpty());
}

/*
 * call-seq:
 *   val.null?  => true or false
 *
 * Returns <code>true</code> when value is a javascipt null.
 *
 */
static VALUE rb_v8_value_null_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsNull());
}

/*
 * call-seq:
 *   val.undefined?  => true or false
 *
 * Returns <code>true</code> when value is undefined.
 *
 */
static VALUE rb_v8_value_undefined_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->IsUndefined());
}

/*
 * call-seq:
 *   val.to_string  => str
 *
 * Returns string representation of this value.
 *
 */
static VALUE rb_v8_value_to_string(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToString());
}

/*
 * call-seq:
 *   val.to_integer  => int
 *
 * Returns integer representation of this value.
 *
 */
static VALUE rb_v8_value_to_integer(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToInteger());
}

/*
 * call-seq:
 *   val.to_number  => num
 *
 * Returns number representation of this value.
 *
 */
static VALUE rb_v8_value_to_number(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToNumber());
}

/*
 * call-seq:
 *   val.to_boolean  => bool
 *
 * Returns bool representation of this value.
 *
 */
static VALUE rb_v8_value_to_boolean(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->ToBoolean());
}

/*
 * call-seq:
 *   val.to_object  => obj
 *
 * Returns object representation of this value.
 *
 */
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
  rb_define_method(rb_cV8Value, "obj?", RUBY_METHOD_FUNC(rb_v8_value_object_p), 0);
  rb_define_method(rb_cV8Value, "integer?", RUBY_METHOD_FUNC(rb_v8_value_integer_p), 0);
  rb_define_method(rb_cV8Value, "int?", RUBY_METHOD_FUNC(rb_v8_value_integer_p), 0);
  rb_define_method(rb_cV8Value, "string?", RUBY_METHOD_FUNC(rb_v8_value_string_p), 0);
  rb_define_method(rb_cV8Value, "str?", RUBY_METHOD_FUNC(rb_v8_value_string_p), 0);
  rb_define_method(rb_cV8Value, "number?", RUBY_METHOD_FUNC(rb_v8_value_number_p), 0);
  rb_define_method(rb_cV8Value, "num?", RUBY_METHOD_FUNC(rb_v8_value_number_p), 0);
  rb_define_method(rb_cV8Value, "external?", RUBY_METHOD_FUNC(rb_v8_value_external_p), 0);
  rb_define_method(rb_cV8Value, "array?", RUBY_METHOD_FUNC(rb_v8_value_array_p), 0);
  rb_define_method(rb_cV8Value, "ary?", RUBY_METHOD_FUNC(rb_v8_value_array_p), 0);
  rb_define_method(rb_cV8Value, "function?", RUBY_METHOD_FUNC(rb_v8_value_function_p), 0);
  rb_define_method(rb_cV8Value, "func?", RUBY_METHOD_FUNC(rb_v8_value_function_p), 0);
  rb_define_method(rb_cV8Value, "boolean?", RUBY_METHOD_FUNC(rb_v8_value_boolean_p), 0);
  rb_define_method(rb_cV8Value, "bool?", RUBY_METHOD_FUNC(rb_v8_value_boolean_p), 0);
  rb_define_method(rb_cV8Value, "regexp?", RUBY_METHOD_FUNC(rb_v8_value_regexp_p), 0);
  rb_define_method(rb_cV8Value, "regex?", RUBY_METHOD_FUNC(rb_v8_value_regexp_p), 0);
  rb_define_method(rb_cV8Value, "date?", RUBY_METHOD_FUNC(rb_v8_value_date_p), 0);
  rb_define_method(rb_cV8Value, "true?", RUBY_METHOD_FUNC(rb_v8_value_true_p), 0);
  rb_define_method(rb_cV8Value, "false?", RUBY_METHOD_FUNC(rb_v8_value_false_p), 0);
  rb_define_method(rb_cV8Value, "empty?", RUBY_METHOD_FUNC(rb_v8_value_empty_p), 0);
  rb_define_method(rb_cV8Value, "undefined?", RUBY_METHOD_FUNC(rb_v8_value_undefined_p), 0);
  rb_define_method(rb_cV8Value, "null?", RUBY_METHOD_FUNC(rb_v8_value_null_p), 0);
  rb_define_method(rb_cV8Value, "to_string", RUBY_METHOD_FUNC(rb_v8_value_to_string), 0);
  rb_define_method(rb_cV8Value, "to_integer", RUBY_METHOD_FUNC(rb_v8_value_to_integer), 0);
  rb_define_method(rb_cV8Value, "to_number", RUBY_METHOD_FUNC(rb_v8_value_to_number), 0);
  rb_define_method(rb_cV8Value, "to_object", RUBY_METHOD_FUNC(rb_v8_value_to_object), 0);
  rb_define_method(rb_cV8Value, "to_boolean", RUBY_METHOD_FUNC(rb_v8_value_to_boolean), 0);
  rb_define_method(rb_cV8Value, "===", RUBY_METHOD_FUNC(rb_v8_value_strict_equals), 1);
  rb_define_method(rb_cV8Value, "==", RUBY_METHOD_FUNC(rb_v8_value_equals), 1);
}

/* V8::Primitive initializer. */
void Init_V8_Primitive()
{
  rb_cV8Primitive = rb_define_class_under(rb_mV8, "Primitive", rb_cV8Value);
}
