#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_string.h"

using namespace v8;

VALUE rb_cMustangV8Object;

/* Local helpers */

Handle<Object> unwrap(VALUE value)
{
  return v8_ref_get<Object>(value);
}

/* Mustang::V8::Object methods */

static VALUE rb_v8_object_new(VALUE self)
{
  HandleScope scope;
  return v8_ref_new(self, Object::New());
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


/* Mustang::V8::Object class initializer. */
void Init_V8_Object()
{
  rb_cMustangV8Object = rb_define_class_under(rb_mMustangV8, "Object", rb_cObject);
  rb_define_singleton_method(rb_cMustangV8Object, "new", RUBY_METHOD_FUNC(rb_v8_object_new), 0);
  rb_define_method(rb_cMustangV8Object, "[]", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cMustangV8Object, "get", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cMustangV8Object, "[]=", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cMustangV8Object, "set", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cMustangV8Object, "keys", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
  rb_define_method(rb_cMustangV8Object, "properties", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
}
