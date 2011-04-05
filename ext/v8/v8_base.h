#ifndef __V8_BASE_H
#define __V8_BASE_H

#include "v8_main.h"

using namespace v8;

/* V8::Data class */
RUBY_EXTERN VALUE rb_cV8Data;
/* V8::EmptyClass class */
RUBY_EXTERN VALUE rb_cV8EmptyClass;
/* V8::UndefinedClass class */
RUBY_EXTERN VALUE rb_cV8UndefinedClass;
/* V8::NullClass class */
RUBY_EXTERN VALUE rb_cV8NullClass;

/* API */
void Init_V8_Data();
void Init_V8_Empty();
void Init_V8_Undefined();
void Init_V8_Null();

#endif /* __V8_BASE_H */
