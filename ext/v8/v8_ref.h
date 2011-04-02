#ifndef __V8_REF_H
#define __V8_REF_H

#include <ruby.h>
#include <v8.h>

using namespace v8;

/*
 * The objectRef keeps a persistent V8 handle, so ruby object can access
 * a reference to it.
 *
 */
struct v8_ref {
  v8_ref(Handle<void> object);
  virtual ~v8_ref();
  void set(const char *name, VALUE ref);
  Persistent<void> handle;
  VALUE references;
};

/* API */
VALUE v8_ref_new(VALUE obj, Handle<void> handle);
void v8_ref_set(VALUE obj, const char *name, VALUE ref);

/*
 * Gets reference to V8 object from related ruby object, and reflects
 * it to specified type.
 *
 *   v8_ref_get<String>(rb_str_value);
 *   v8_ref_get<Integer>(rb_int_value);
 *
 */
template <class T> Local<T> v8_ref_get(VALUE obj)
{
  v8_ref* r = 0;
  Data_Get_Struct(obj, struct v8_ref, r);
  return (T*)*r->handle;
}

#endif//__V8_REF_H
