#ifndef __V8_MAIN_H
#define __V8_MAIN_H

#include <ruby.h>
#include <v8.h>

/* V8 module */
RUBY_EXTERN VALUE rb_mV8;
RUBY_EXTERN VALUE rb_cV8Data;
RUBY_EXTERN VALUE rb_cV8Empty;
RUBY_EXTERN VALUE rb_cV8Undefined;
RUBY_EXTERN VALUE rb_cV8Null;

/* API */
void Init_V8();

#endif//__V8_MAIN_H
