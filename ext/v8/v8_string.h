#ifndef __V8_STRING_H
#define __V8_STRING_H

#include "v8_main.h"

using namespace v8;

/* V8::String class */
RUBY_EXTERN VALUE rb_cV8String;

/* API */
VALUE v8_string_cast(Handle<Value> value);
Handle<Value> v8_string_cast(VALUE value);
void Init_V8_String();

#endif//__V8_STRING_H
