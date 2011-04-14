#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_external.h"

using namespace v8;

void rb_v8_wrapper_gc_mark(rb_sV8Wrapper *r)
{
  rb_gc_mark(r->references);
}

void rb_v8_wrapper_gc_free(rb_sV8Wrapper *r)
{
  delete r;
}

/*
 * Creates new reference to V8 object for ruby object.
 *
 *   rb_v8_wrapper_new(rb_cV8Integer, Integer::New());
 *   rb_v8_wrapper_new(rb_cV8String, String::Cast(*value));
 *
 */
VALUE rb_v8_wrapper_new(VALUE klass, Handle<void> handle)
{
  rb_sV8Wrapper *r = new rb_sV8Wrapper(handle);
  return Data_Wrap_Struct(klass, rb_v8_wrapper_gc_mark, rb_v8_wrapper_gc_free, r);
}

/*
 * Appends additional reference to ruby object wrapped with v8_wrapper.
 *
 */
void rb_v8_wrapper_aset(VALUE obj, const char *name, VALUE ref)
{
  rb_sV8Wrapper *r = 0;
  Data_Get_Struct(obj, struct rb_sV8Wrapper, r);
  r->set(name, ref);
}

/*
 * Returns specified referenced ruby object wrapped with v8_wrapper. 
 *
 */
VALUE rb_v8_wrapper_aref(VALUE obj, const char *name)
{
  rb_sV8Wrapper *r = 0;
  Data_Get_Struct(obj, struct rb_sV8Wrapper, r);
  return r->get(name);
}

/* The v8_wrapper struct methods. */

rb_sV8Wrapper::rb_sV8Wrapper(Handle<void> object)
{
  handle = Persistent<void>::New(object);
  references = rb_hash_new();
}

rb_sV8Wrapper::~rb_sV8Wrapper()
{
  handle.Dispose();
}

void rb_sV8Wrapper::set(const char *name, VALUE ref)
{
  if (ref != 0 && RTEST(ref) && !NIL_P(ref)) {
    rb_hash_aset(references, rb_str_new2(name), ref);
  }
}

VALUE rb_sV8Wrapper::get(const char *name)
{
  return rb_hash_aref(references, rb_str_new2(name));
}
