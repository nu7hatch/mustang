#ifndef __V8_OBJECT_H
#define __V8_OBJECT_H

#include "v8_main.h"

using namespace v8;

/* Mustang::V8::Object class */
RUBY_EXTERN VALUE rb_cMustangV8Object;

/* API */
VALUE v8_object_cast(Handle<Value> value);
void Init_V8_Object();

#endif//__V8_OBJECT_H
