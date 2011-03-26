#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_function.h"
#include "v8_context.h"

using namespace v8;

Handle<Value> to_v8(VALUE value)
{
  switch (TYPE(value)) {
  case T_NIL:
    Null();
  case T_TRUE:
    True();
  case T_FALSE:
    False();
  case T_FIXNUM:
    return v8_integer_cast(value);
  case T_STRING:
    return v8_string_cast(value);
  case T_SYMBOL:
    return to_v8(rb_sym_to_s(value));
  default:
    return Undefined();
  }
}

VALUE to_ruby(Handle<Value> value)
{
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  }
  if (value->IsUint32() || value->IsInt32()) {
    return v8_integer_cast(value);
  }
  if (value->IsString()) {
    return v8_string_cast(value);
  }
  if (value->IsFunction()) {
    return v8_function_cast(value);
  }

  return Qnil;
}

OVERLOADED_V8_TO_RUBY_CAST(Object);
OVERLOADED_V8_TO_RUBY_CAST(String);
OVERLOADED_V8_TO_RUBY_CAST(Function);
OVERLOADED_V8_TO_RUBY_CAST(Array);

VALUE to_ruby(bool value)     { return value ? Qtrue : Qfalse; }
VALUE to_ruby(double value)   { return rb_float_new(value); }
VALUE to_ruby(int64_t value)  { return LONG2NUM(value); }
VALUE to_ruby(uint32_t value) { return UINT2NUM(value); }
VALUE to_ruby(int32_t value)  { return INT2FIX(value); }
VALUE to_ruby(char *value)    { return rb_str_new2(value); }
