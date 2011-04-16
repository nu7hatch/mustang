#ifndef __V8_MACROS_H
#define __V8_MACROS_H

#ifndef rb_cMethod
#define rb_cMethod rb_const_get(rb_cObject, rb_intern("Method"))
#endif

#ifndef HAVE_RB_SYM_TO_S
#define rb_sym_to_s(sym) rb_funcall2(sym, rb_intern("to_s"), 0, NULL)
#endif

#ifndef HAVE_RB_ANY_TO_ARY
#define rb_any_to_ary(range) rb_funcall2(value, rb_intern("to_a"), 0, NULL)
#endif

#ifndef RUBY_EXTERN
#define RUBY_EXTERN extern
#endif

#ifdef RUBINIUS
typedef unsigned int int_r;
#else
typedef int int_r;
#endif

/* Automaticly declares unwrap method for given type */
#define UNWRAPPER(T)				\
  static Local<T> unwrap(VALUE self)		\
  {						\
    return v8_handle_from_wrapper<T>(self);					\
  }

/* Checks if V8 is within some context, and raises proper exception if not. */
#define PREVENT_CREATION_WITHOUT_CONTEXT()				                \
  if (!Context::InContext()) {					                        \
    rb_raise(rb_eRuntimeError, "can't create V8 object without entering into context");	\
    return Qnil;							                \
  }				       

#endif//__V8_MACROS_H
