#ifndef __V8_ERRORS_H
#define __V8_ERRORS_H

#include "v8_main.h"

using namespace v8;

RUBY_EXTERN VALUE rb_eV8Error;
RUBY_EXTERN VALUE rb_eV8RangeError;
RUBY_EXTERN VALUE rb_eV8ReferenceError;
RUBY_EXTERN VALUE rb_eV8StntaxError;

/* API */
VALUE rb_v8_error_new2(Handle<Value> ex, Handle<Message> msg);
VALUE rb_v8_error_new3(TryCatch try_catch);
void Init_V8_Errors();

#endif /* __V8_ERRORS_H */
