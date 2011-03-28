#include "v8_main.h"

using namespace v8;

VALUE rb_mV8;

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


/* V8 module initializer. */
void Init_V8()
{
  rb_mV8 = rb_define_module("V8");
  rb_define_singleton_method(rb_mV8, "dead?", RUBY_METHOD_FUNC(rb_v8_dead_p), 0);
  rb_define_singleton_method(rb_mV8, "alive?", RUBY_METHOD_FUNC(rb_v8_alive_p), 0);
  rb_define_singleton_method(rb_mV8, "version", RUBY_METHOD_FUNC(rb_v8_version), 0);
}
