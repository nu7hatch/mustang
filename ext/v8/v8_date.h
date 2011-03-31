#ifndef __V8_DATE_H
#define __V8_DATE_H

#include "v8_main.h"

using namespace v8;

/* V8::Date class */
RUBY_EXTERN VALUE rb_cV8Date;

/* API */
Handle<Value> to_v8_date(VALUE value);
VALUE rb_v8_date_new2(VALUE data);
void Init_V8_Date();

#endif//__V8_DATE_H
