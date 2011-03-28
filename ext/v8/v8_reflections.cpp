#include "v8_reflections.h"

using namespace v8;
using namespace std;

#define defHandle2RbReflection(from)		\
  VALUE handle2rb(from value)			\
  {						\
    return handle2rb((Handle<Value>) value);	\
  }

/* Array converters between ruby value and v8 object. */
namespace arrayRef
{
  Handle<Value> to_v8(VALUE value)
  {
    HandleScope scope;
    long int arylen = RARRAY_LEN(value);
    Local<Array> v8ary = Array::New(arylen);

    for (int i = 0; i < arylen; i++) {
      v8ary->Set(i, rb2handle(rb_ary_entry(value, i)));
    }

    return scope.Close(v8ary);
  }

  VALUE to_rb(Handle<Value> value)
  {
    HandleScope scope;
    Local<Array> v8ary(Array::Cast(*value));
    VALUE rbary = rb_ary_new();

    for (unsigned int i = 0; i < v8ary->Length(); i++) {
      rb_ary_push(rbary, handle2rb(v8ary->Get(i)));
    }

    return rbary;
  }
}

/* Hash converter to v8 object. */
namespace hashRef
{
  Handle<Value> to_v8(VALUE value)
  {
    HandleScope scope;
    Local<Object> obj = Object::New();

    VALUE keys = rb_funcall2(value, rb_intern("keys"), 0, NULL);

    for (int i = 0; i < RARRAY_LEN(keys); i++) {
      VALUE key = rb_ary_entry(keys, i);
      VALUE val = rb_hash_aref(value, key);

      obj->Set(rb2handle(key), rb2handle(val));
    }
    
    return scope.Close(obj);
  }
}

namespace objectRef
{
  VALUE to_rb(Handle<Value> value)
  {
    HandleScope scope;
    Local<Object> v8obj(Object::Cast(*value));
    Local<Array> properties = v8obj->GetPropertyNames();

    VALUE rbobj = rb_hash_new();
    
    for (unsigned int i = 0; i < properties->Length(); i++) {
      Local<Value> name = properties->Get(i);
      rb_hash_aset(rbobj, handle2rb(name), handle2rb(v8obj->Get(name)));
    }

    return rbobj;
  }
}

/* Function/Procs converters. */
namespace functionRef
{
  struct func {
    Handle<Function> func;
  };
  
  Handle<Value> to_v8(VALUE value)
  {
    return Undefined();
  }

  VALUE to_rb(Handle<Value> value)
  {
    HandleScope scope;
    //Local<Function> v8func = Function::Cast(*value);
    struct func *f = new func();
    //f->func = v8func;
    return Data_Wrap_Struct(rb_cProc, 0, 0, f);
  }
}

/* String converters between ruby value and v8 object. */
namespace stringRef
{
  Handle<Value> to_v8(VALUE value)
  {
    return String::New(StringValuePtr(value));
  }

  VALUE to_rb(Handle<Value> value)
  {
    HandleScope scope;
    String::Utf8Value str(String::Cast(*value)->ToString());
    return rb_str_new2(string(*str).c_str());
  }
}

/* Floating point numbers converters between ruby value and v8 object. */
namespace numberRef
{
  Handle<Value> to_v8(VALUE value)
  {
    return Number::New(NUM2DBL(value));
  }

  VALUE to_rb(Handle<Value> value)
  {
    return rb_float_new(value->NumberValue());
  }
}

/* Integers converters between ruby value and v8 object. */
namespace integerRef
{
  Handle<Value> to_v8(VALUE value)
  {
    return Integer::New(FIX2LONG(value));
  }

  VALUE int32_to_rb(Handle<Value> value)
  {
    return INT2FIX(value->Int32Value());
  }

  VALUE uint32_to_rb(Handle<Value> value)
  {
    return UINT2NUM(value->Uint32Value());
  }
}

/* Boolean values converters between ruby value and v8 object. */
namespace booleanRef
{
  VALUE to_rb(Handle<Value> value)
  {
    return value->BooleanValue() ? Qtrue : Qfalse;
  }
}

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
  case T_SYMBOL:
    //return rb2handle(rb_sym_to_s(value));
  case T_STRING:
    return stringRef::to_v8(value);
  case T_FLOAT:
    return numberRef::to_v8(value);
  case T_FIXNUM:
    return integerRef::to_v8(value);
  case T_ARRAY:
    return arrayRef::to_v8(value);
  case T_HASH:
    return hashRef::to_v8(value);
  default:
    rb_warn("Can't convert ruby object to V8: %s", RSTRING_PTR(rb_inspect(value)));
    return Undefined();
  }
}

/* Converts V8 value to ruby compilant one. */
VALUE handle2rb(Handle<Value> value)
{
  if (value.IsEmpty() || value->IsUndefined() || value->IsNull()) {
    return Qnil;
  }
  if (value->IsUint32()) {
    return integerRef::uint32_to_rb(value);
  }
  if (value->IsInt32()) {
    return integerRef::int32_to_rb(value);
  }
  if (value->IsBoolean()) {
    return booleanRef::to_rb(value);
  }
  if (value->IsNumber()) {
    return numberRef::to_rb(value);
  }
  if (value->IsString()) {
    return stringRef::to_rb(value);
  }
  if (value->IsArray()) {
    return arrayRef::to_rb(value);
  }
  //if (value->IsExternal()) {}
  if (value->IsFunction()) {
    return functionRef::to_rb(value);
  }
  if (value->IsObject()) {
    return objectRef::to_rb(value);
  }
  
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
defHandle2RbReflection(Handle<Function>);
defHandle2RbReflection(Handle<Object>);
defHandle2RbReflection(Handle<Array>);
defHandle2RbReflection(Handle<Integer>);
defHandle2RbReflection(Handle<Uint32>);
defHandle2RbReflection(Handle<Int32>);
