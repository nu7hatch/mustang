#include "v8_runtime.h"

static VALUE rb_mMustang;
static VALUE rb_mMustangV8;

extern "C" void Init_v8() {
  rb_mMustang = rb_define_module("Mustang");
  rb_mMustangV8 = rb_define_module_under(rb_mMustang, "V8");

  initRuntime(rb_mMustangV8);
}
