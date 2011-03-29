#ifndef __V8_NUMBER_H
#define __V8_NUMBER_H

#include "v8_main.h"

using namespace v8;

/* V8::Number class */
RUBY_EXTERN VALUE rb_cV8Number;

/* API */
Handle<Value> to_v8_number(VALUE value);
VALUE rb_v8_number_new2(VALUE data);
void Init_V8_Number();

#endif//__V8_NUMBER_H
