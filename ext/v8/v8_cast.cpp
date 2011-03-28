#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_number.h"
#include "v8_array.h"
#include "v8_function.h"
#include "v8_context.h"
#include "v8_external.h"
#include "v8_macros.h"

using namespace v8;

Handle<Value> to_v8(VALUE value)
{
  switch (TYPE(value)) {
  case T_NIL:
    return Null();
  case T_TRUE:
    return True();
  case T_FALSE:
    return False();
  case T_SYMBOL:
    return to_v8(rb_sym_to_s(value));
  case T_FIXNUM:
    return v8_integer_cast(value);
  case T_FLOAT:
    return v8_number_cast(value);
  case T_STRING:
    return v8_string_cast(value);
  case T_ARRAY:
    return v8_array_cast(value);
  case T_HASH:
    return v8_object_cast(value);
  default:
    if (rb_obj_is_kind_of(value, rb_cV8String)) {
      return v8_ref_get<String>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8Integer)) {
      return v8_ref_get<Integer>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8Array)) {
      return v8_ref_get<Array>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8Function)) {
      return v8_ref_get<Function>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8Object)) {
      return v8_ref_get<Object>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8External)) {
      return v8_ref_get<External>(value);
    } else {
      return v8_external_cast(value);
    }
  }
}

VALUE to_ruby(Handle<Value> value)
{
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  } else if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  } else if (value->IsUint32() || value->IsInt32()) {
    return v8_integer_cast(value);
  } else if (value->IsNumber()) {
    return v8_number_cast(value);
  } else if (value->IsString()) {
    return v8_string_cast(value);
  } else if (value->IsFunction()) {
    return v8_function_cast(value);
  } else if (value->IsArray()) {
    return v8_array_cast(value);
  } else if (value->IsExternal()) {
    return v8_external_cast(value);
  } else if (value->IsObject()) {
    return v8_object_cast(value);
  }

  return Qnil;
}

OVERLOADED_V8_TO_RUBY_CAST(Object);
OVERLOADED_V8_TO_RUBY_CAST(String);
OVERLOADED_V8_TO_RUBY_CAST(Integer);
OVERLOADED_V8_TO_RUBY_CAST(Int32);
OVERLOADED_V8_TO_RUBY_CAST(Uint32);
OVERLOADED_V8_TO_RUBY_CAST(Function);
OVERLOADED_V8_TO_RUBY_CAST(Array);
OVERLOADED_V8_TO_RUBY_CAST(External);

VALUE to_ruby(bool value)     { return value ? Qtrue : Qfalse; }
VALUE to_ruby(double value)   { return rb_float_new(value); }
VALUE to_ruby(char *value)    { return rb_str_new2(value); }
VALUE to_ruby(int64_t value)  { return LONG2NUM(value); }
VALUE to_ruby(uint32_t value) { return UINT2NUM(value); }
VALUE to_ruby(int32_t value)  { return INT2FIX(value); }

/* V8::Cast module methods */

VALUE rb_mV8Cast;

/*
 * call-seq:
 *   obj.to_v8  => v8_obj
 *
 * Converts standard ruby object to ruby v8 representation.
 *
 *   [1,2,3].to_v8 # => #<V8::Array>
 *   1230.to_v8    # => #<V8::Integer>
 *   "foo".to_v8   # => #<V8::String>
 *
 */
static VALUE rb_v8_cast_to_v8(VALUE self)
{
  HandleScope scope;
  return to_ruby(to_v8(self));
}


/* V8::Cast module initializer */
void Init_V8_Cast()
{
  rb_mV8Cast = rb_define_module_under(rb_mV8, "Cast");
  rb_define_method(rb_mV8Cast, "to_v8", RUBY_METHOD_FUNC(rb_v8_cast_to_v8), 0);
}
