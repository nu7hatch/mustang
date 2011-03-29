#ifndef __V8_MACROS_H
#define __V8_MACROS_H

/* Symbol to string cast method for ruby < 1.9 */
#ifndef rb_sym_to_s
#define rb_sym_to_s(sym) rb_funcall2(sym, rb_intern("to_s"), 0, NULL)
#endif

/* Name of hidden value which contains ruby peer object. */
#define RUBY_PEER_ATTR "__RUBY_PEER__"

/* Automaticly declares unwrap method for given type */
#define UNWRAPPER(T)				\
  static Local<T> unwrap(VALUE self)		\
  {						\
    return v8_ref_get<T>(self);			\
  }						\

#endif//__V8_MACROS_H
