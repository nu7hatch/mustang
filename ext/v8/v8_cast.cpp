#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_base.h"
#include "v8_value.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_number.h"
#include "v8_date.h"
#include "v8_array.h"
#include "v8_function.h"
#include "v8_regexp.h"
#include "v8_context.h"
#include "v8_external.h"
#include "v8_errors.h"
#include "v8_macros.h"

using namespace v8;

#define OVERLOAD_TO_RUBY_WITH(from)		   \
  VALUE to_ruby(Handle<from> value)		   \
  {						   \
    return to_ruby((Handle<Value>) value);	   \
  }

Handle<Value> to_v8(VALUE value)
{
  HandleScope scope;
  
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
    return to_v8(rb_v8_integer_new2(value));
  case T_FLOAT:
    return to_v8(rb_v8_number_new2(value));
  case T_STRING:
    return to_v8(rb_v8_string_new2(value));
  case T_REGEXP:
    return to_v8(rb_v8_regexp_new2(value));
  case T_ARRAY:
    return to_v8(rb_v8_array_new2(value));
  case T_HASH:
    return to_v8(rb_v8_object_new2(value));
  default:
    if (rb_obj_is_kind_of(value, rb_cRange)) {
      return to_v8(rb_any_to_ary(value));
    } else if (rb_obj_is_kind_of(value, rb_cTime)) {
      return to_v8(rb_v8_date_new2(value));
    } else if (rb_obj_is_kind_of(value, rb_cProc)) {
      return to_v8(rb_v8_function_new2(value));
    } if (rb_obj_is_kind_of(value, rb_cV8Value)) {
      return v8_ref_get<Value>(value);
    } else if (rb_obj_is_kind_of(value, rb_cV8Undefined)) {
      return Undefined();
    } else if (rb_obj_is_kind_of(value, rb_cV8Null)) {
      return Null();
    } else if (rb_obj_is_kind_of(value, rb_cV8Empty)) {
      return Handle<Value>();
    } else {
      return to_v8(rb_v8_external_new2(value));
    }
  }
}

VALUE to_ruby(Handle<Value> value)
{
  if (value.IsEmpty()) {
    return rb_funcall(rb_cV8Empty, rb_intern("new"), 0, NULL);
  } else if (value->IsUndefined()) {
    return rb_funcall(rb_cV8Undefined, rb_intern("new"), 0, NULL);
  } else if (value->IsNull()) {
    return rb_funcall(rb_cV8Null, rb_intern("new"), 0, NULL);
  } else if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  } else if (value->IsDate()) {
    return to_ruby_without_peer<Date>(value, rb_cV8Date);
  } else if (value->IsUint32() || value->IsInt32()) {
    return to_ruby_without_peer<Integer>(value, rb_cV8Integer);
  } else if (value->IsNumber()) {
    return to_ruby_without_peer<Number>(value, rb_cV8Number);
  } else if (value->IsString()) {
    return to_ruby_without_peer<String>(value, rb_cV8String);
  } else if (value->IsFunction()) {
    return to_ruby_with_peer<Function>(value, rb_cV8Function);
  } else if (value->IsRegExp()) {
    return to_ruby_without_peer<RegExp>(value, rb_cV8Regexp);
  } else if (value->IsArray()) {
    return to_ruby_with_peer<Array>(value, rb_cV8Array);
  } else if (value->IsExternal()) {
    return to_ruby_without_peer<External>(value, rb_cV8External);
  } else if (value->IsObject()) {
    return to_ruby_with_peer<Object>(value, rb_cV8Object);
  }

  return Qnil;
}

OVERLOAD_TO_RUBY_WITH(Boolean);
OVERLOAD_TO_RUBY_WITH(String);
OVERLOAD_TO_RUBY_WITH(Integer);
OVERLOAD_TO_RUBY_WITH(Number);
OVERLOAD_TO_RUBY_WITH(Function);
OVERLOAD_TO_RUBY_WITH(RegExp);
OVERLOAD_TO_RUBY_WITH(Array);
OVERLOAD_TO_RUBY_WITH(External);
OVERLOAD_TO_RUBY_WITH(Object);

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
