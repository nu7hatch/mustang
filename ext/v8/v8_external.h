#ifndef __V8_EXTERNAL_H
#define __V8_EXTERNAL_H

#include "v8_main.h"

using namespace v8;

/* V8::External class */
RUBY_EXTERN VALUE rb_cV8External;

/* API */
Handle<Value> to_v8_external(VALUE value);
VALUE rb_v8_external_new2(VALUE data);
void Init_V8_External();

#endif//__V8_EXTERNAL_H
