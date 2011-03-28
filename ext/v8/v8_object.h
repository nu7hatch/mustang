#ifndef __V8_OBJECT_H
#define __V8_OBJECT_H

#include "v8_main.h"

using namespace v8;

/* V8::Object class */
RUBY_EXTERN VALUE rb_cV8Object;

/* API */
Handle<Value> v8_object_cast(VALUE value);
VALUE v8_object_cast(Handle<Value> value);
void Init_V8_Object();

#endif//__V8_OBJECT_H
