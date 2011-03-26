#ifndef __V8_MAIN_H
#define __V8_MAIN_H

#include <ruby.h>
#include <v8.h>

/* Mustang module */
RUBY_EXTERN VALUE rb_mMustang;
/* Mustang::V8 module */
RUBY_EXTERN VALUE rb_mMustangV8;

/* API */
void Init_V8();

#endif//__V8_MAIN_H
