#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"

using namespace v8;

VALUE rb_cV8Object;

/* Typecasting helpers */

VALUE v8_object_cast(Handle<Value> value)
{
  HandleScope scope;
  Local<Object> obj = Object::Cast(*value);
  return v8_ref_new(rb_cV8Object, obj);
}

Handle<Value> v8_object_cast(VALUE value)
{
  HandleScope scope;
  Local<Object> obj = Object::New();    
  VALUE hsh = rb_funcall2(value, rb_intern("to_hash"), 0, NULL);
  VALUE keys = rb_funcall2(value, rb_intern("keys"), 0, NULL);

  // TODO: for sure there is more efficient way to retrieve all hash
  // values and keys, some enumerator/iterator?
  for (int i = 0; i < RARRAY_LEN(keys); i++) {
    VALUE key = rb_ary_entry(keys, i);
    obj->Set(to_v8(key)->ToString(), to_v8(rb_hash_aref(hsh, key)));
  }
  
  return scope.Close(obj);
}

/* Local helpers */

static Handle<Object> unwrap(VALUE value)
{
  return v8_ref_get<Object>(value);
}

/* V8::Object methods */

static VALUE rb_v8_object_new(int argc, VALUE *argv, VALUE self)
{
  HandleScope scope;

  switch (argc) {
  case 0:
    return v8_ref_new(self, Object::New());
  case 1:
    return v8_ref_new(self, v8_object_cast(argv[0]));
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    return Qnil;
  }
}

/*
 * call-seq:
 *   obj[key]      => value
 *   obj.get(key)  => value
 *
 * Returns value of specified object's property. If property is undefined
 * then returns <code>nil</code>.
 *
 *   obj = cxt.evaluate("{foo: 'bar'};")
 *   obj["foo"] # => 'bar'
 *
 */
static VALUE rb_v8_object_get(VALUE self, VALUE key)
{
  HandleScope scope;

  if (FIXNUM_P(key)) {
    return to_ruby(unwrap(self)->Get(NUM2UINT(key)));
  } else {
    return to_ruby(unwrap(self)->Get(to_v8(key)->ToString()));
  }
}

/*
 * call-seq:
 *   obj[key] = value     => value
 *   obj.set(key, value)  => value
 *
 * Sets given value as specified property in current object.
 *
 *   obj = cxt.evaluate("var obj = {foo: 'bar'}; obj;")
 *   obj["bar"] = "foo"
 *   cxt.evaluate("obj.bar") # => 'foo'
 *
 */
static VALUE rb_v8_object_set(VALUE self, VALUE key, VALUE value)
{
  HandleScope scope;
  Handle<Value> _value = to_v8(value);

  if (FIXNUM_P(key)) {
    unwrap(self)->Set(NUM2UINT(key), _value);
  } else {
    unwrap(self)->Set(to_v8(key)->ToString(), _value);
  }
  
  return to_ruby(_value);
}

/*
 * call-seq:
 *   obj.keys        => array
 *   obj.properties  => array
 *
 * Returns list of property names belonging to an object. 
 *
 *   obj = cxt.evaluate("{foo: 'bar', spam: 'eggs'};")
 *   obj.keys # => ['foo', 'spam']
 *
 */
static VALUE rb_v8_object_keys(VALUE self)
{
  HandleScope scope;
  Handle<Array> v8keys = unwrap(self)->GetPropertyNames();
  VALUE keys = rb_ary_new();
  
  for (unsigned int i = 0; i < v8keys->Length(); i++) {
    rb_ary_push(keys, to_ruby(v8keys->Get(i)));
  }

  return keys;
}


/* V8::Object class initializer. */
void Init_V8_Object()
{
  rb_cV8Object = rb_define_class_under(rb_mV8, "Object", rb_cObject);
  rb_define_singleton_method(rb_cV8Object, "new", RUBY_METHOD_FUNC(rb_v8_object_new), -1);
  rb_define_method(rb_cV8Object, "[]", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cV8Object, "get", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cV8Object, "[]=", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cV8Object, "set", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cV8Object, "keys", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
  rb_define_method(rb_cV8Object, "properties", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
}
