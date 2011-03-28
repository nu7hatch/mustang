#ifndef __V8_CAST_H
#define __V8_CAST_H

#include "v8_main.h"

using namespace v8;

/* V8::Cast module */
RUBY_EXTERN VALUE rb_mV8Cast;

#define OVERLOADED_V8_TO_RUBY_CAST(from)	   \
  VALUE to_ruby(Handle<from> value)		   \
  {						   \
    return to_ruby((Handle<Value>) value);	   \
  }

/* API */
Handle<Value> to_v8(VALUE value);

VALUE to_ruby(Handle<Value> value);
VALUE to_ruby(Handle<Object> value);
VALUE to_ruby(Handle<String> value);
VALUE to_ruby(Handle<Function> value);
VALUE to_ruby(Handle<Array> value);

VALUE to_ruby(bool value);
VALUE to_ruby(double value);
VALUE to_ruby(int64_t value);
VALUE to_ruby(uint32_t value);
VALUE to_ruby(int32_t value);
VALUE to_ruby(char *value);

void Init_V8_Cast();

#endif//__V8_CAST_H
