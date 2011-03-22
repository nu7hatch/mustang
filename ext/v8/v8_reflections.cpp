#include "v8_reflections.h"

using namespace v8;
using namespace std;

#define defHandle2RbReflection(from)		\
  VALUE handle2rb(from value)			\
  {						\
    return handle2rb((Handle<Value>) value);	\
  }

// TODO: add missing coversions for both sides...

/* Converts given ruby value to V8 compilant handle. If given value
 * can't be converted then returns `Undefined` and shows appropriate
 * warning message. */
Handle<Value> rb2handle(VALUE value)
{
  switch (TYPE(value)) {
  case T_NIL:
    return Null();
  case T_TRUE:
    return True();
  case T_FALSE:
    return False();
  case T_STRING:
    return String::New(StringValuePtr(value));
  case T_FLOAT:
    return Number::New(NUM2DBL(value));
  case T_FIXNUM:
    return Integer::New(FIX2LONG(value));
  default:
    rb_warn("Can't convert ruby object to V8: %s", RSTRING_PTR(rb_inspect(value)));
    return Undefined();
  }
}

/* Converts V8 string to ruby value. */
VALUE v8string2rb(Handle<Value> value)
{
  HandleScope scope;
  String::Utf8Value str(String::Cast(*value)->ToString());
  return rb_str_new2(string(*str).c_str());
}

/* Converts V8 value to ruby compilant one. */
VALUE handle2rb(Handle<Value> value)
{
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsUint32()) {
    return UINT2NUM(value->Uint32Value());
  }
  if (value->IsInt32()) {
    return INT2FIX(value->Int32Value());
  }
  if (value->IsBoolean()) {
    return value->BooleanValue() ? Qtrue : Qfalse;
  }
  if (value->IsNumber()) {
    return rb_float_new(value->NumberValue());
  }
  if (value->IsString()) {
    return v8string2rb(value);
  }
  //if (value->IsExternal()) {}
  //if (value->IsFunction()) {}
  //if (value->IsArray()) {}
  //if (value->IsDate()) {}
  //if (value->IsObject()) {}
  
  return Qnil;
}

/* Overloaded convertes for various v8 data types */

VALUE handle2rb(bool value)
{
  return value ? Qtrue : Qfalse;
}

VALUE handle2rb(double value)
{
  return rb_float_new(value);
}

VALUE handle2rb(int64_t value)
{
  return LONG2NUM(value);
}

VALUE handle2rb(uint32_t value)
{
  return UINT2NUM(value);
}

VALUE handle2rb(int32_t value)
{
  return INT2FIX(value);
}

defHandle2RbReflection(Handle<Boolean>);
defHandle2RbReflection(Handle<Number>);
defHandle2RbReflection(Handle<String>);
defHandle2RbReflection(Handle<Object>);
defHandle2RbReflection(Handle<Array>);
defHandle2RbReflection(Handle<Function>);
defHandle2RbReflection(Handle<Integer>);
defHandle2RbReflection(Handle<Uint32>);
defHandle2RbReflection(Handle<Int32>);
