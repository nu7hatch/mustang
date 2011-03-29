#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_external.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8External;
UNWRAPPER(External);

/* Typecasting helpers */

Handle<Value> to_v8_external(VALUE value)
{
  return External::New((void*)value);
}

/* V8::External methods */

static VALUE rb_v8_external_new(VALUE klass, VALUE obj)
{
  HandleScope scope;
  return v8_ref_new(klass, to_v8_external(obj));
}
  
static VALUE rb_v8_external_value(VALUE self)
{
  HandleScope scope;
  return (VALUE)unwrap(self)->Value();
}

/* Public constructors */

VALUE rb_v8_external_new2(VALUE data)
{
  return rb_v8_external_new(rb_cV8External, data);
}


/* V8::External initializer. */
void Init_V8_External()
{
  // TODO: exchange parent class with rb_cV8Value when ready...
  rb_cV8External = rb_define_class_under(rb_mV8, "External", rb_cV8Value);
  rb_define_singleton_method(rb_cV8External, "new", RUBY_METHOD_FUNC(rb_v8_external_new), 1);
  rb_define_method(rb_cV8External, "value", RUBY_METHOD_FUNC(rb_v8_external_value), 0);
  rb_define_method(rb_cV8External, "to_obj", RUBY_METHOD_FUNC(rb_v8_external_value), 0);
}
