#include "v8_main.h"
#include "v8_cast.h"
#include "v8-debug.h"

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

/*
 * call-seq:
 *   V8.enable_debug!(port)
 *
 * Enables v8 debugger on specified port. 
 *
 */
static VALUE rb_v8_debugger_bang(VALUE self, VALUE port)
{
  return to_ruby(Debug::EnableAgent("V8 debugger", NUM2INT(port), false));
}


/* V8 module initializer. */
void Init_V8()
{
  VALUE rb_mMustang = rb_define_module("Mustang");
  
  rb_mV8 = rb_define_module_under(rb_mMustang, "V8");
  rb_define_singleton_method(rb_mV8, "dead?", RUBY_METHOD_FUNC(rb_v8_dead_p), 0);
  rb_define_singleton_method(rb_mV8, "alive?", RUBY_METHOD_FUNC(rb_v8_alive_p), 0);
  rb_define_singleton_method(rb_mV8, "debugger!", RUBY_METHOD_FUNC(rb_v8_debugger_bang), 1);
  rb_define_singleton_method(rb_mV8, "version", RUBY_METHOD_FUNC(rb_v8_version), 0);
}
