#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_external.h"

using namespace v8;

VALUE rb_cV8External;

/* Typecasting helpers */

static VALUE references;

void v8_gc_week_reference_callback(Persistent<Value> object, void* parameter) {
  Local<External> external = External::Cast(*object);
  rb_hash_delete(references, rb_obj_id((VALUE)external->Value()));
}

Handle<Value> v8_external_cast(VALUE value)
{
  HandleScope scope;
  //rb_hash_aset(references, rb_obj_id(value), value);
  Local<Value> external = External::New((void*)value);
  //Persistent<Value> record = Persistent<Value>::New(external);
  //record.MakeWeak(NULL, v8_gc_week_reference_callback);
  return scope.Close(external);
}

VALUE v8_external_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<External> ext = External::Cast(*value);
  return v8_ref_new(rb_cV8External, ext);
}

/* Local helpers */

static Handle<External> unwrap(VALUE value)
{
  return v8_ref_get<External>(value); 
}

/* V8::External methods */

static VALUE rb_v8_external_new(VALUE klass, VALUE obj)
{
  HandleScope scope;
  return v8_ref_new(klass, v8_external_cast(obj));
}
  
static VALUE rb_v8_external_value(VALUE self)
{
  HandleScope scope;
  return (VALUE)unwrap(self)->Value();
}


/* V8::External initializer. */
void Init_V8_External()
{
  // TODO: exchange parent class with rb_cV8Value when ready...
  rb_cV8External = rb_define_class_under(rb_mV8, "External", rb_cObject);
  rb_define_singleton_method(rb_cV8External, "new", RUBY_METHOD_FUNC(rb_v8_external_new), 1);
  rb_define_method(rb_cV8External, "value", RUBY_METHOD_FUNC(rb_v8_external_value), 0);
  rb_define_method(rb_cV8External, "to_obj", RUBY_METHOD_FUNC(rb_v8_external_value), 0);
}
