#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Array;
UNWRAPPER(Array);

/* Typecasting helpers */

Handle<Value> to_v8_array(VALUE value)
{
  HandleScope scope;
  Local<Array> ary = Array::New();

  for (int_r i = 0; i < RARRAY_LEN(value); i++) {
    ary->Set(i, to_v8(rb_ary_entry(value, i)));
  }
  
  return ary;
}

/* V8::Array methods */

/*
 * call-seq:
 *   V8::Array.new         => new_array
 *   V8::Array.new(array)  => new_array
 *
 * Returns a new V8 array.
 *
 */
static VALUE rb_v8_array_new(VALUE klass, VALUE data)
{
  HandleScope scope;
  PREVENT_CREATION_WITHOUT_CONTEXT();
  
  VALUE ary = rb_funcall2(data, rb_intern("to_a"), 0, NULL);
  VALUE self = rb_v8_wrapper_new(klass, to_v8_array(ary));

  v8_set_peer(self);
  return self;
}

/*
 * call-seq:
 *   ary.to_a  => array
 *
 * Returns referenced array data represented as ruby array.
 *
 */
static VALUE rb_v8_array_to_a(VALUE self)
{
  HandleScope scope;
  VALUE ary = rb_ary_new();
  Handle<Array> v8ary = unwrap(self);
  
  for (unsigned int i = 0; i < v8ary->Length(); i++) {
    rb_ary_store(ary, i, to_ruby(v8ary->Get(i)));
  }

  return ary;
}

/*
 * call-seq:
 *   ary[key]      => value
 *   ary.get(key)  => value
 *
 * Returns value of specified array entry.
 *
 *   ary = cxt.evaluate("['foo', 'bar'];")
 *   ary[0] # => 'foo'
 *
 */
static VALUE rb_v8_array_get(VALUE self, VALUE key)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->Get(NUM2UINT(key)));
}

/*
 * call-seq:
 *   ary[key] = value     => value
 *   ary.set(key, value)  => value
 *
 * Sets given value under specified key.
 *
 */
static VALUE rb_v8_array_set(VALUE self, VALUE key, VALUE value)
{
  HandleScope scope;
  Handle<Value> _value = to_v8(value);
  unwrap(self)->Set(NUM2UINT(key), _value);  
  return to_ruby(_value);
}

/*
 * call-seq:
 *   ary.push(value)  => value
 *   ary << value     => value
 *
 * Appends given value to referenced array.
 *
 */
static VALUE rb_v8_array_push(VALUE self, VALUE value)
{
  HandleScope scope;
  Handle<Value> _value = to_v8(value);
  Handle<Array> ary = unwrap(self);
  ary->Set(ary->Length(), _value);  
  return to_ruby(_value);
}

// TODO: #pop
// TODO: #delete
// TODO: #delete_key

/*
 * call-seq:
 *   ary.length  => size
 *   ary.size    => size
 *
 * Returns number of items stored in this array.
 *
 */
VALUE rb_v8_array_length(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->Length());
}

/* Public constructors */

VALUE rb_v8_array_new2(VALUE data)
{
  return rb_v8_array_new(rb_cV8Array, data);
}

VALUE rb_v8_array_new3()
{
  return rb_v8_array_new(rb_cV8Array, rb_ary_new());
}


/* V8::Array initializer. */
void Init_V8_Array()
{
  rb_cV8Array = rb_define_class_under(rb_mV8, "Array", rb_cV8Object);
  rb_define_singleton_method(rb_cV8Array, "new", RUBY_METHOD_FUNC(rb_v8_array_new), -2);
  rb_define_method(rb_cV8Array, "to_a", RUBY_METHOD_FUNC(rb_v8_array_to_a), 0);
  rb_define_method(rb_cV8Array, "[]", RUBY_METHOD_FUNC(rb_v8_array_get), 1);
  rb_define_method(rb_cV8Array, "get", RUBY_METHOD_FUNC(rb_v8_array_get), 1);
  rb_define_method(rb_cV8Array, "[]=", RUBY_METHOD_FUNC(rb_v8_array_set), 2);
  rb_define_method(rb_cV8Array, "set", RUBY_METHOD_FUNC(rb_v8_array_set), 2);
  rb_define_method(rb_cV8Array, "push", RUBY_METHOD_FUNC(rb_v8_array_push), 1);
  rb_define_method(rb_cV8Array, "<<", RUBY_METHOD_FUNC(rb_v8_array_push), 1);
  rb_define_method(rb_cV8Array, "length", RUBY_METHOD_FUNC(rb_v8_array_length), 0);
  rb_define_method(rb_cV8Array, "size", RUBY_METHOD_FUNC(rb_v8_array_length), 0);  
}
