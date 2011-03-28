#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_number.h"

using namespace v8;

VALUE rb_cV8Number;

/* Typecasting */

VALUE v8_number_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<Number> num = Number::Cast(*value);
  return v8_ref_new(rb_cV8Number, num);
}

Handle<Value> v8_number_cast(VALUE value)
{
  HandleScope scope;
  return scope.Close(Number::New(NUM2DBL(value)));
}

/* Local helpers */

static Local<Number> unwrap(VALUE self)
{
  return v8_ref_get<Number>(self);
}  

/* V8::Number methods */

static VALUE rb_v8_number_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  VALUE num = rb_funcall2(data, rb_intern("to_f"), 0, NULL);
  return v8_ref_new(klass, v8_number_cast(num));
}

/*
 * call-seq:
 *   str.to_f  => value
 *
 * Returns float value representation of referenced v8 number.
 *
 */
static VALUE rb_v8_number_to_f(VALUE self)
{
  HandleScope scope;
  return rb_float_new(unwrap(self)->NumberValue());
}


/* V8::Number initializer. */
void Init_V8_Number()
{
  rb_cV8Number = rb_define_class_under(rb_mV8, "Number", rb_cV8Object);
  rb_define_singleton_method(rb_cV8Number, "new", RUBY_METHOD_FUNC(rb_v8_number_new), 1);
  rb_define_method(rb_cV8Number, "to_f", RUBY_METHOD_FUNC(rb_v8_number_to_f), 0);
}
