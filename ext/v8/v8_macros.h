#ifndef __V8_MACROS_H
#define __V8_MACROS_H

/* Symbol to string cast method for ruby < 1.9 */
#ifndef rb_sym_to_s
#define rb_sym_to_s(sym) \
  rb_funcall2(sym, rb_intern("to_s"), 0, NULL)
#endif

/* Converts any object to array by triggering #to_a on it. */
#define rb_any_to_ary(range) \
  rb_funcall2(value, rb_intern("to_a"), 0, NULL)

/* Name of hidden value which contains ruby peer object. */
#define RUBY_PEER_ATTR "__RUBY_PEER__"

/* Assigns ruby peer object as hidden value of reflected v8 object. */
#define v8_set_peer2(handle, obj) \
  handle->SetHiddenValue(String::New(RUBY_PEER_ATTR), External::Wrap((void*)obj))
#define v8_set_peer(obj) \
  v8_set_peer2(unwrap(obj), obj)

/* Automaticly declares unwrap method for given type */
#define UNWRAPPER(T)				\
  static Local<T> unwrap(VALUE self)		\
  {						\
    return v8_ref_get<T>(self);			\
  }

#define PREVENT_CREATION_WITHOUT_CONTEXT()				                \
  if (!Context::InContext()) {					                        \
    rb_raise(rb_eRuntimeError, "can't create V8 object without entering into context");	\
    return Qnil;							                \
  }				       

#endif//__V8_MACROS_H
