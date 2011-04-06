#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_date.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Date;
UNWRAPPER(Date);

/* Typecasting */

Handle<Value> to_v8_date(VALUE value)
{
  return Date::New(NUM2DBL(value));
}

/* V8::Date methods */

/*
 * call-seq:
 *   V8::Date.new(time)  => new_date
 *
 * Returns new V8 date reflected from given ruby time value.
 *
 */
static VALUE rb_v8_date_new(VALUE klass, VALUE time)
{
  HandleScope scope;
  PREVENT_CREATION_WITHOUT_CONTEXT();  
  return v8_ref_new(klass, to_v8_date(time));
}

/*
 * call-seq:
 *   str.to_i  => value
 *
 * Returns integer value representation of referenced v8 date.
 *
 */
static VALUE rb_v8_date_to_i(VALUE self)
{
  HandleScope scope;
  return rb_int_new(unwrap(self)->NumberValue());
}

/* Public constructors */

VALUE rb_v8_date_new2(VALUE data)
{
  return rb_v8_date_new(rb_cV8Date, data);
}


/* V8::Date initializer. */
void Init_V8_Date()
{
  rb_cV8Date = rb_define_class_under(rb_mV8, "Date", rb_cV8Value);
  rb_define_singleton_method(rb_cV8Date, "new", RUBY_METHOD_FUNC(rb_v8_date_new), 1);
  rb_define_method(rb_cV8Date, "to_i", RUBY_METHOD_FUNC(rb_v8_date_to_i), 0);
}
