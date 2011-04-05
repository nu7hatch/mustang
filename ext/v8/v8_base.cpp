#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_base.h"
#include "v8_errors.h"
#include "v8_value.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Data;
VALUE rb_cV8EmptyClass;
VALUE rb_cV8UndefinedClass;
VALUE rb_cV8NullClass;

/* V8::Data methods */

/*
 * call-seq:
 *   data.value?  => true or false
 *
 * Returns <code>true</code> when value is a javascript value..
 *
 */
static VALUE rb_v8_data_value_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8Value);
}

/*
 * call-seq:
 *   data.error?  => true or false
 *
 * Returns <code>true</code> when value is a handled javascipt exception.
 *
 */
static VALUE rb_v8_data_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8Error);
}

/*
 * call-seq:
 *   data.null?  => true or false
 *
 * Returns <code>true</code> when value is a javascipt null.
 *
 */
static VALUE rb_v8_data_null_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8NullClass);
}

/*
 * call-seq:
 *   data.empty?  => true or false
 *
 * Returns <code>true</code> when value is empty.
 *
 */
static VALUE rb_v8_data_empty_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8EmptyClass);
}

/*
 * call-seq:
 *   data.undefined?  => true or false
 *
 * Returns <code>true</code> when value is undefined.
 *
 */
static VALUE rb_v8_data_undefined_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_cV8UndefinedClass);
}

/* V8::Null methods */

/*
 * call-seq:
 *   null.to_s  => "NULL"
 *
 */
static VALUE rb_v8_null_to_s(VALUE self)
{
  return rb_str_new2("NULL");
}

/* V8::Empty methods */

/*
 * call-seq:
 *   empty.to_s  => "Empty"
 *
 */
static VALUE rb_v8_empty_to_s(VALUE self)
{
  return rb_str_new2("Empty");
}

/* V8::Undefined methods */

/*
 * call-seq:
 *   undefined.to_s  => "Undefined"
 *
 */
static VALUE rb_v8_undefined_to_s(VALUE self)
{
  return rb_str_new2("Undefined");
}


/* V8::Data initializer */
void Init_V8_Data()
{  
  rb_cV8Data = rb_define_class_under(rb_mV8, "Data", rb_cObject);
  rb_define_method(rb_cV8Data, "error?", RUBY_METHOD_FUNC(rb_v8_data_error_p), 0);
  rb_define_method(rb_cV8Data, "null?", RUBY_METHOD_FUNC(rb_v8_data_null_p), 0);
  rb_define_method(rb_cV8Data, "empty?", RUBY_METHOD_FUNC(rb_v8_data_empty_p), 0);
  rb_define_method(rb_cV8Data, "undefined?", RUBY_METHOD_FUNC(rb_v8_data_undefined_p), 0);
  rb_define_method(rb_cV8Data, "value?", RUBY_METHOD_FUNC(rb_v8_data_value_p), 0);
}

/* V8::Empty initializer */
void Init_V8_Empty()
{
  rb_cV8EmptyClass = rb_define_class_under(rb_mV8, "EmptyClass", rb_cV8Data);
  rb_define_method(rb_cV8EmptyClass, "to_s", RUBY_METHOD_FUNC(rb_v8_empty_to_s), 0);
  rb_define_const(rb_mV8, "Empty", rb_funcall2(rb_cV8EmptyClass, rb_intern("new"), 0, NULL));
}

/* V8::Undefined initializer */
void Init_V8_Undefined()
{
  rb_cV8UndefinedClass = rb_define_class_under(rb_mV8, "UndefinedClass", rb_cV8Data);
  rb_define_method(rb_cV8UndefinedClass, "to_s", RUBY_METHOD_FUNC(rb_v8_undefined_to_s), 0);
  rb_define_const(rb_mV8, "Undefined", rb_funcall2(rb_cV8UndefinedClass, rb_intern("new"), 0, NULL));
}

/* V8::Null initializer */
void Init_V8_Null()
{
  rb_cV8NullClass = rb_define_class_under(rb_mV8, "NullClass", rb_cV8Data);
  rb_define_method(rb_cV8NullClass, "to_s", RUBY_METHOD_FUNC(rb_v8_null_to_s), 0);  
  rb_define_const(rb_mV8, "Null", rb_funcall2(rb_cV8NullClass, rb_intern("new"), 0, NULL));
}
