#ifndef __V8_BASE_H
#define __V8_BASE_H

#include "v8_main.h"

using namespace v8;

/* V8::Data class */
RUBY_EXTERN VALUE rb_cV8Data;
/* V8::Empty singleton */
RUBY_EXTERN VALUE rb_cV8Empty;
/* V8::Undefined singleton */
RUBY_EXTERN VALUE rb_cV8Undefined;
/* V8::Null singleton */
RUBY_EXTERN VALUE rb_cV8Null;

/* API */
void Init_V8_Data();
void Init_V8_Empty();
void Init_V8_Undefined();
void Init_V8_Null();

#endif /* __V8_BASE_H */
