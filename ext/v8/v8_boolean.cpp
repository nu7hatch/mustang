#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_boolean.h"
#include "v8_errors.h"
#include "v8_value.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Boolean;
VALUE rb_cV8FalseClass;
VALUE rb_cV8TrueClass;

/* Booleans initializer */
void Init_V8_Boolean()
{
  rb_cV8Boolean = rb_define_class_under(rb_mV8, "Boolean", rb_cV8Primitive);
  VALUE args;
  
  /* false */
  args = Qfalse;
  rb_cV8FalseClass = rb_define_class_under(rb_mV8, "FalseClass", rb_cV8Boolean);
  rb_define_const(rb_mV8, "False", rb_funcall2(rb_cV8FalseClass, rb_intern("new"), 1, &args));

  /* true */
  args = Qtrue;
  rb_cV8TrueClass = rb_define_class_under(rb_mV8, "TrueClass", rb_cV8Boolean);
  rb_define_const(rb_mV8, "True", rb_funcall2(rb_cV8TrueClass, rb_intern("new"), 1, &args));
}
