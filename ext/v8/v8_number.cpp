#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_number.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Number;
UNWRAPPER(Number);

/* Typecasting */

Handle<Value> to_v8_number(VALUE value)
{
  return Number::New(NUM2DBL(value));
}

/* V8::Number methods */

/*
 * call-seq:
 *   V8::Number.new(float)  => new_number
 *
 * Returns new V8 number reflected from given float value.
 *
 */
static VALUE rb_v8_number_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  PREVENT_CREATION_WITHOUT_CONTEXT();
  VALUE num = rb_funcall2(data, rb_intern("to_f"), 0, NULL);
  return v8_ref_new(klass, to_v8_number(num));
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

/* Public constructors */

VALUE rb_v8_number_new2(VALUE data)
{
  return rb_v8_number_new(rb_cV8Number, data);
}


/* V8::Number initializer. */
void Init_V8_Number()
{
  rb_cV8Number = rb_define_class_under(rb_mV8, "Number", rb_cV8Primitive);
  rb_define_singleton_method(rb_cV8Number, "new", RUBY_METHOD_FUNC(rb_v8_number_new), 1);
  rb_define_method(rb_cV8Number, "to_f", RUBY_METHOD_FUNC(rb_v8_number_to_f), 0);
}
