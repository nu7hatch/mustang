#include "v8_main.h"

using namespace v8;

VALUE rb_mSingleton = rb_eval_string("require 'singleton'; Singleton");

VALUE rb_mV8;
VALUE rb_cV8Data;
VALUE rb_cV8Empty;
VALUE rb_cV8Undefined;
VALUE rb_cV8Null;

/* V8 singleton methods. */

/*
 * call-seq:
 *   V8.dead?  => value
 *
 * Returns <code>true</code> when V8 engine is down.
 *
 */
static VALUE rb_v8_dead_p(VALUE self)
{
  return V8::IsDead() ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   V8.alive?  => value
 *
 * Returns <code>true</code> wile V8 engine is running.
 *
 */
static VALUE rb_v8_alive_p(VALUE self)
{
  return V8::IsDead() ? Qfalse : Qtrue;
}

/*
 * call-seq:
 *   V8.dead?  => value
 *
 * Returns used version of V8 engine.
 *
 */
static VALUE rb_v8_version(VALUE self)
{
  return rb_str_new2(V8::GetVersion());
}

static VALUE rb_v8_null_to_s(VALUE self)
{
  return rb_str_new2("NULL");
}

static VALUE rb_v8_empty_to_s(VALUE self)
{
  return rb_str_new2("Empty");
}

static VALUE rb_v8_undefined_to_s(VALUE self)
{
  return rb_str_new2("Undefined");
}

static VALUE rb_v8_data_null_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8Null);
}

static VALUE rb_v8_data_empty_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8Empty);
}

static VALUE rb_v8_data_undefined_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8Undefined);
}

/* V8 module initializer. */
void Init_V8()
{
  rb_mV8 = rb_define_module("V8");
  rb_define_singleton_method(rb_mV8, "dead?", RUBY_METHOD_FUNC(rb_v8_dead_p), 0);
  rb_define_singleton_method(rb_mV8, "alive?", RUBY_METHOD_FUNC(rb_v8_alive_p), 0);
  rb_define_singleton_method(rb_mV8, "version", RUBY_METHOD_FUNC(rb_v8_version), 0);

  rb_cV8Data = rb_define_class_under(rb_mV8, "Data", rb_cObject);
  rb_define_method(rb_cV8Data, "null?", RUBY_METHOD_FUNC(rb_v8_data_null_p), 0);
  rb_define_method(rb_cV8Data, "empty?", RUBY_METHOD_FUNC(rb_v8_data_empty_p), 0);
  rb_define_method(rb_cV8Data, "undefined?", RUBY_METHOD_FUNC(rb_v8_data_undefined_p), 0);
  
  rb_cV8Empty = rb_define_class_under(rb_mV8, "Empty", rb_cV8Data);
  rb_include_module(rb_cV8Empty, rb_mSingleton);
  rb_define_method(rb_cV8Empty, "to_s", RUBY_METHOD_FUNC(rb_v8_empty_to_s), 0);
  
  rb_cV8Null = rb_define_class_under(rb_mV8, "Null", rb_cV8Data);
  rb_include_module(rb_cV8Null, rb_mSingleton);
  rb_define_method(rb_cV8Null, "to_s", RUBY_METHOD_FUNC(rb_v8_null_to_s), 0);

  rb_cV8Undefined = rb_define_class_under(rb_mV8, "Undefined", rb_cV8Data);
  rb_include_module(rb_cV8Undefined, rb_mSingleton);
  rb_define_method(rb_cV8Undefined, "to_s", RUBY_METHOD_FUNC(rb_v8_undefined_to_s), 0);
}
