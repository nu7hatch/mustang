#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_object.h"
#include "v8_value.h"
#include "v8_string.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Object;
UNWRAPPER(Object);

/* Typecasting helpers */

Handle<Value> to_v8_object(VALUE value)
{
  HandleScope scope;
  Local<Object> obj = Object::New();

  if (rb_obj_is_kind_of(value, rb_cHash)) {
    VALUE keys = rb_funcall2(value, rb_intern("keys"), 0, NULL);

    // TODO: for sure there is more efficient way to retrieve all hash
    // values and keys, some enumerator/iterator?
    for (int i = 0; i < RARRAY_LEN(keys); i++) {
      VALUE key = rb_ary_entry(keys, i);
      obj->Set(to_v8(key)->ToString(), to_v8(rb_hash_aref(value, key)));
    }
  } else {
    obj->SetHiddenValue(String::New("RUBY_OBJECT"), External::Wrap((void*)value));

    // Reflection of objects different than hash will be done on the ruby side.
    // See `lib/v8/object.rb` for details...
  }
  
  return obj;
}

/* V8::Object methods */

/*
 * call-seq:
 *   V8::Object.new
 *   V8::Object.new(hash)  => new_object
 *
 * Returns new V8 object.
 *
 */
static VALUE rb_v8_object_new(int argc, VALUE *argv, VALUE klass)
{
  HandleScope scope;
  VALUE self;
  
  switch (argc) {
  case 0:
    self = v8_ref_new(klass, Object::New());
    break;
  case 1:
    self = v8_ref_new(klass, to_v8_object(argv[0]));
    rb_iv_set(self, "@origin", argv[0]);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    return Qnil;
  }

  v8_set_peer(self);
  return self;
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

/* Public constructors */

VALUE rb_v8_object_new2(VALUE data)
{
  return rb_v8_object_new(1, &data, rb_cV8Object);
}

VALUE rb_v8_object_new3()
{
  return rb_v8_object_new(0, NULL, rb_cV8Object);
}


/* V8::Object class initializer. */
void Init_V8_Object()
{
  rb_cV8Object = rb_define_class_under(rb_mV8, "Object", rb_cV8Value);
  rb_define_singleton_method(rb_cV8Object, "new", RUBY_METHOD_FUNC(rb_v8_object_new), -1);
  rb_define_method(rb_cV8Object, "[]", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cV8Object, "get", RUBY_METHOD_FUNC(rb_v8_object_get), 1);
  rb_define_method(rb_cV8Object, "[]=", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cV8Object, "set", RUBY_METHOD_FUNC(rb_v8_object_set), 2);
  rb_define_method(rb_cV8Object, "keys", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
  rb_define_method(rb_cV8Object, "properties", RUBY_METHOD_FUNC(rb_v8_object_keys), 0);
}
