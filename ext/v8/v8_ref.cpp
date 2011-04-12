#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_external.h"

using namespace v8;

void gc_v8_object_mark(v8_ref *r)
{
  if (!NIL_P(r->origin)) {
    rb_gc_mark(r->origin);
  }

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
VALUE v8_ref_new(VALUE klass, Handle<void> handle, VALUE orig/* = Qnil */)
{
  v8_ref *r = new v8_ref(handle, orig);
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

/*
 * Returns original object from which v8 reflection has been created.
 *
 */
VALUE v8_ref_orig(VALUE obj)
{
  v8_ref *r = 0;
  Data_Get_Struct(obj, struct v8_ref, r);
  return r->origin;
}

/* The v8_ref struct methods. */

v8_ref::v8_ref(Handle<void> object, VALUE orig/* = Qnil */)
{
  handle = Persistent<void>::New(object);
  references = rb_hash_new();
  origin = orig;
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
