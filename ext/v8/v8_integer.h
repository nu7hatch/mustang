#ifndef __V8_INTEGER_H
#define __V8_INTEGER_H

#include "v8_main.h"

using namespace v8;

/* V8::Integer class */
RUBY_EXTERN VALUE rb_cV8Integer;

/* API */
Handle<Value> to_v8_integer(VALUE value);
VALUE rb_v8_integer_new2(VALUE data);
void Init_V8_Integer();

#endif//__V8_INTEGER_H
