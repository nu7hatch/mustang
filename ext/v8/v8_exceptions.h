#ifndef __V8_EXCEPTIONS_H
#define __V8_EXCEPTIONS_H

#include "v8_main.h"

using namespace v8;

RUBY_EXTERN VALUE rb_eV8Exception;
RUBY_EXTERN VALUE rb_eV8Error;
RUBY_EXTERN VALUE rb_eV8RangeError;
RUBY_EXTERN VALUE rb_eV8ReferenceError;
RUBY_EXTERN VALUE rb_eV8StntaxError;

/* API */
VALUE rb_v8_exception_new2(TryCatch *try_catch);
void Init_V8_Exceptions();

#endif /* __V8_EXCEPTIONS_H */
