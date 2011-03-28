#ifndef __V8_FUNCTION_H
#define __V8_FUNCTION_H

#include "v8_main.h"

/* V8::Function class */
RUBY_EXTERN VALUE rb_cV8Function;

/* API */
VALUE v8_function_cast(v8::Handle<v8::Value> value);
void Init_V8_Function();

#endif//__V8_FUNCTION_H
