#include "v8_runtime.h"
#include "v8_reflections.h"

static VALUE rb_mMustang;
static VALUE rb_mMustangV8;

/* Returns `true` when V8 engine is down. */
static VALUE v8IsDead(VALUE self)
{
  return handle2rb(V8::IsDead());
}

/* Returns used version of V8 engine. */
static VALUE v8Version(VALUE self)
{
  return rb_str_new2(V8::GetVersion());
}

extern "C" void Init_v8() {
  rb_mMustang = rb_define_module("Mustang");

  rb_mMustangV8 = rb_define_module_under(rb_mMustang, "V8");
  rb_define_singleton_method(rb_mMustangV8, "dead?", RUBY_METHOD_FUNC(v8IsDead), 0);
  rb_define_singleton_method(rb_mMustangV8, "version", RUBY_METHOD_FUNC(v8Version), 0);
  
  initRuntime(rb_mMustangV8);
}
