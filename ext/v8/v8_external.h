#ifndef __V8_EXTERNAL_H
#define __V8_EXTERNAL_H

#include "v8_main.h"

using namespace v8;

/* V8::External class */
RUBY_EXTERN VALUE rb_cV8External;

/* API */
VALUE v8_external_cast(Handle<Value> value);
Handle<Value> v8_external_cast(VALUE value);
void Init_V8_External();

#endif//__V8_EXTERNAL_H
