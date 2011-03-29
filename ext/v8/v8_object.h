#ifndef __V8_OBJECT_H
#define __V8_OBJECT_H

#include "v8_main.h"

using namespace v8;

/* V8::Object class */
RUBY_EXTERN VALUE rb_cV8Object;

/* API */
Handle<Value> to_v8_object(VALUE value);
VALUE rb_v8_object_new2(VALUE data);
VALUE rb_v8_object_new3();
void Init_V8_Object();

#endif//__V8_OBJECT_H
