#ifndef __V8_FUNCTION_H
#define __V8_FUNCTION_H

#include "v8_main.h"

/* V8::Function class */
RUBY_EXTERN VALUE rb_cV8Function;

/* API */
VALUE to_v8_function(v8::Handle<v8::Value> value);
VALUE rb_v8_function_new2(VALUE data);
void Init_V8_Function();

#endif//__V8_FUNCTION_H
