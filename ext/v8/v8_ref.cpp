#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_external.h"

using namespace v8;

void gc_v8_object_mark(v8_ref *r)
{
  rb_gc_mark(r->references);
}

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
VALUE v8_ref_new(VALUE klass, Handle<void> handle)
{
  v8_ref *r = new v8_ref(handle);
  return Data_Wrap_Struct(klass, gc_v8_object_mark, gc_v8_object_free, r);
}

/*
 * Appends additional reference to ruby object wrapped with v8_ref.
 *
 */
void v8_ref_set(VALUE obj, const char *name, VALUE ref)
{
  v8_ref *r = 0;
  Data_Get_Struct(obj, struct v8_ref, r);
  r->set(name, ref);
}

/* The v8_ref struct methods. */

v8_ref::v8_ref(Handle<void> object)
{
  handle = Persistent<void>::New(object);
  references = rb_hash_new();
}

v8_ref::~v8_ref()
{
  handle.Dispose();
}

void v8_ref::set(const char *name, VALUE ref)
{
  if (ref != 0 && RTEST(ref)) {
    rb_hash_aset(references, rb_str_new2(name), ref);
  }
}
