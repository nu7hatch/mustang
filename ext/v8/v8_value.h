#ifndef __V8_VALUE_H
#define __V8_VALUE_H

#include "v8_main.h"

using namespace v8;

/* V8::Value class */
RUBY_EXTERN VALUE rb_cV8Value;
/* V8::Primitive class */
RUBY_EXTERN VALUE rb_cV8Primitive;

/* API */
Handle<Value> to_v8_value(VALUE value);
VALUE rb_v8_value_new2(VALUE data);
void Init_V8_Value();
void Init_V8_Primitive();

#endif//__V8_VALUE_H
