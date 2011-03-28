#ifndef __V8_INTEGER_H
#define __V8_INTEGER_H

#include "v8_main.h"

using namespace v8;

/* V8::Integer class */
RUBY_EXTERN VALUE rb_cV8Integer;

/* API */
VALUE v8_integer_cast(Handle<Value> value);
Handle<Value> v8_integer_cast(VALUE value);
void Init_V8_Integer();

#endif//__V8_INTEGER_H
