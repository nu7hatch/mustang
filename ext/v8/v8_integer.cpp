#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_integer.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Integer;
UNWRAPPER(Integer);

/* Typecasting */

Handle<Value> to_v8_integer(VALUE value)
{
  return Integer::New(FIX2LONG(value));
}

/* V8::Integer methods */

/*
 * call-seq:
 *   V8::Integer.new(int)  => new_integer
 *
 * Returns new V8 integer reflected from given fixnum value.
 *
 */
static VALUE rb_v8_integer_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  PREVENT_CREATION_WITHOUT_CONTEXT();
  VALUE num = rb_funcall2(data, rb_intern("to_i"), 0, NULL);
  return rb_v8_wrapper_new(klass, to_v8_integer(num));
}

/*
 * call-seq:
 *   str.to_i  => value
 *
 * Returns fixnum value representation of referenced v8 integer.
 *
 */
static VALUE rb_v8_integer_to_i(VALUE self)
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

/* Public constructors */

VALUE rb_v8_integer_new2(VALUE data)
{
  return rb_v8_integer_new(rb_cV8Integer, data);
}


/* V8::Integer initializer. */
void Init_V8_Integer()
{
  rb_cV8Integer = rb_define_class_under(rb_mV8, "Integer", rb_cV8Primitive);
  rb_define_singleton_method(rb_cV8Integer, "new", RUBY_METHOD_FUNC(rb_v8_integer_new), 1);
  rb_define_method(rb_cV8Integer, "to_i", RUBY_METHOD_FUNC(rb_v8_integer_to_i), 0);
}
