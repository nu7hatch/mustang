#include "v8_ref.h"

using namespace v8;

void gc_v8_object_free(v8_ref *r)
{
  delete r;
}

/*
 * Creates new reference to V8 object for ruby object.
 *
 *   v8_ref_new(rb_cV8Integer, Integer::New());
 *   v8_ref_new(rb_cV8String, String::Cast(*value));
 *
 */
VALUE v8_ref_new(VALUE obj, Handle<void> handle)
{
  v8_ref *r = new v8_ref(handle);
  return Data_Wrap_Struct(obj, 0, gc_v8_object_free, r);
}

/* The v8_ref struct methods. */

v8_ref::v8_ref(Handle<void> object)
{
  handle = Persistent<void>::New(object);
}

v8_ref::~v8_ref()
{
  handle.Dispose();
}
