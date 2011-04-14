#ifndef __V8_WRAPPER_H
#define __V8_WRAPPER_H

#include <ruby.h>
#include <v8.h>

using namespace v8;

/* Names of hidden values which contains ruby peer object. */
#define RUBY_PEER_ATTR "__RUBY_PEER__"

/* Assigns ruby peer object as hidden value of reflected v8 object. */
#define v8_set_peer2(handle, obj) \
  handle->SetHiddenValue(String::New(RUBY_PEER_ATTR), External::Wrap((void*)obj))
#define v8_set_peer(obj) \
  v8_set_peer2(unwrap(obj), obj)

/*
 * The objectRef keeps a persistent V8 handle, so ruby object can access
 * a reference to it.
 *
 */
struct rb_sV8Wrapper {
  rb_sV8Wrapper(Handle<void> object);
  virtual ~rb_sV8Wrapper();
  void set(const char *name, VALUE ref);
  VALUE get(const char *name);
  Persistent<void> handle;
  VALUE references;
};

/* API */
VALUE rb_v8_wrapper_new(VALUE obj, Handle<void> handle);
void rb_v8_wrapper_aset(VALUE obj, const char *name, VALUE ref);
VALUE rb_v8_wrapper_aref(VALUE obj, const char *name);

/*
 * Gets reference to V8 object from related ruby object, and reflects
 * it to specified type.
 *
 *   v8_handle_from_wrapper<String>(rb_str_value);
 *   v8_handle_from_wrapper<Integer>(rb_int_value);
 *
 */
template <class T> Local<T> v8_handle_from_wrapper(VALUE obj)
{
  rb_sV8Wrapper* r = 0;
  Data_Get_Struct(obj, struct rb_sV8Wrapper, r);
  return (T*)*r->handle;
}

#endif//__V8_WRAPPER_H
