#ifndef __V8_BOOLEAN_H
#define __V8_BOOLEAN_H

#include "v8_main.h"

using namespace v8;

/* V8::Boolean class */
RUBY_EXTERN VALUE rb_cV8Boolean;
/* V8::FalseClass class */
RUBY_EXTERN VALUE rb_cV8FalseClass;
/* V8::TrueClass class */
RUBY_EXTERN VALUE rb_cV8TrueClass;

/* API */
void Init_V8_Boolean();

#endif /* __V8_BOOLEAN_H */
