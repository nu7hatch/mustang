#ifndef __V8_NUMBER_H
#define __V8_NUMBER_H

#include "v8_main.h"

using namespace v8;

/* V8::Number class */
RUBY_EXTERN VALUE rb_cV8Number;

/* API */
VALUE v8_number_cast(Handle<Value> value);
Handle<Value> v8_number_cast(VALUE value);
void Init_V8_Number();

#endif//__V8_NUMBER_H
