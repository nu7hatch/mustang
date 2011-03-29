#ifndef __V8_STRING_H
#define __V8_STRING_H

#include "v8_main.h"

using namespace v8;

/* V8::String class */
RUBY_EXTERN VALUE rb_cV8String;

/* API */
Handle<Value> to_v8_string(VALUE value);
VALUE rb_v8_string_new2(VALUE data);
void Init_V8_String();

#endif//__V8_STRING_H
