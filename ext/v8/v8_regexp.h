#ifndef __V8_REGEXP_H
#define __V8_REGEXP_H

#include "v8_main.h"

using namespace v8;

/* V8::Regexp class */
RUBY_EXTERN VALUE rb_cV8Regexp;

/* API */
Handle<Value> to_v8_regexp(VALUE value);
VALUE rb_v8_regexp_new2(VALUE data);
void Init_V8_Regexp();

#endif//__V8_REGEXP_H
