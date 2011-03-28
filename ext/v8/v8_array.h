#ifndef __V8_ARRAY_H
#define __V8_ARRAY_H

#include "v8_main.h"

using namespace v8;

/* V8::Array class */
RUBY_EXTERN VALUE rb_cV8Array;

/* API */
VALUE v8_array_cast(Handle<Value> value);
Handle<Value> v8_array_cast(VALUE value);
void Init_V8_Array();

#endif//__V8_ARRAY_H
