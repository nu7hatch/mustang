#include "v8_main.h"

using namespace v8;

VALUE rb_mMustang;
VALUE rb_mMustangV8;

/* Mustang::V8 singleton methods. */

/*
 * call-seq:
 *   Mustang::V8.dead?  => value
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
 *   Mustang::V8.alive?  => value
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
 *   Mustang::V8.dead?  => value
 *
 * Returns used version of V8 engine.
 *
 */
static VALUE rb_v8_version(VALUE self)
{
  return rb_str_new2(V8::GetVersion());
}


/* Mustang::V8 module initializer. */
void Init_V8()
{
  rb_mMustang = rb_define_module("Mustang");
  rb_mMustangV8 = rb_define_module_under(rb_mMustang, "V8");
  rb_define_singleton_method(rb_mMustangV8, "dead?", RUBY_METHOD_FUNC(rb_v8_dead_p), 0);
  rb_define_singleton_method(rb_mMustangV8, "alive?", RUBY_METHOD_FUNC(rb_v8_alive_p), 0);
  rb_define_singleton_method(rb_mMustangV8, "version", RUBY_METHOD_FUNC(rb_v8_version), 0);
}
