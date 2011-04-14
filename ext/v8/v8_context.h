#ifndef __V8_CONTEXT_H
#define __V8_CONTEXT_H

#include "v8_main.h"

/* V8::Context class */
RUBY_EXTERN VALUE rb_cV8Context;

/* API */
VALUE rb_v8_context_new2(VALUE klass, Handle<Context> context);
void Init_V8_Context();

#endif//__V8_CONTEXT_H
