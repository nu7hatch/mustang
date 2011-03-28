#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_integer.h"

using namespace v8;

VALUE rb_cV8Integer;

/* Typecasting */

VALUE v8_integer_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<Integer> num(Integer::Cast(*value));
  return v8_ref_new(rb_cV8Integer, num);
}

Handle<Value> v8_integer_cast(VALUE value)
{
  HandleScope scope;
  return scope.Close(Integer::New(FIX2LONG(value)));
}

/* Local helpers */

static Local<Integer> unwrap(VALUE self)
{
  return v8_ref_get<Integer>(self);
}  

/* V8::Integer methods */

static VALUE rb_v8_integer_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  VALUE num = rb_funcall2(data, rb_intern("to_i"), 0, NULL);
  return v8_ref_new(klass, v8_integer_cast(num));
}

/*
 * call-seq:
 *   str.to_i  => value
 *
 * Returns fixnum value representation of referenced v8 integer.
 *
 */
static VALUE rb_v8_string_to_i(VALUE self)
{
  HandleScope scope;
  Local<Integer> num = unwrap(self);

  if (num->IsUint32()) {
    return UINT2NUM(num->Uint32Value());
  } else if (num->IsInt32()) {
    return INT2NUM(num->Int32Value());
  } else {
    return INT2NUM(0);
  }
}


/* V8::Integer initializer. */
void Init_V8_Integer()
{
  rb_cV8Integer = rb_define_class_under(rb_mV8, "Integer", rb_cV8Object);
  rb_define_singleton_method(rb_cV8Integer, "new", RUBY_METHOD_FUNC(rb_v8_integer_new), 1);
  rb_define_method(rb_cV8Integer, "to_i", RUBY_METHOD_FUNC(rb_v8_string_to_i), 0);
}
