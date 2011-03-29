#ifndef __V8_ARRAY_H
#define __V8_ARRAY_H

#include "v8_main.h"

using namespace v8;

/* V8::Array class */
RUBY_EXTERN VALUE rb_cV8Array;

/* API */
Handle<Value> to_v8_array(VALUE value);
VALUE rb_v8_array_new2(VALUE data);
VALUE rb_v8_array_new3();
void Init_V8_Array();

#endif//__V8_ARRAY_H
