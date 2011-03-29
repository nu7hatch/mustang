#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_exceptions.h"

using namespace v8;

VALUE rb_eV8Exception;
VALUE rb_eV8Error;
VALUE rb_eV8RangeError;
VALUE rb_eV8ReferenceError;
VALUE rb_eV8SyntaxError;

static VALUE error_types = rb_hash_new();

/* V8 TryCatch reference */

struct v8_try_catch_ref {
  v8_try_catch_ref(TryCatch *tc);
  virtual ~v8_try_catch_ref();
  bool alive;
  TryCatch *try_catch;
};

void gc_v8_try_catch_free(v8_try_catch_ref *r)
{
  if (r != NULL) {
    delete r;
  }
}

v8_try_catch_ref::v8_try_catch_ref(TryCatch *tc)
{
  alive = true;
  try_catch = tc;
}

v8_try_catch_ref::~v8_try_catch_ref()
{
  alive = false;
}

/* Local helpers */

TryCatch* unwrap(VALUE self)
{
  HandleScope scope;
  v8_try_catch_ref *r = 0;
  Data_Get_Struct(self, struct v8_try_catch_ref, r);

  if (r->alive) {
    return r->try_catch;
  } else {
    rb_raise(rb_eScriptError, "out of scope access of %s", RSTRING_PTR(rb_inspect(self)));
    return NULL;
  }
}

Handle<Object> exception(VALUE self)
{
  HandleScope scope;
  Local<Object> obj = Object::Cast(*unwrap(self)->Exception());
  return obj;
}

/* V8::Exception methods */

static VALUE rb_v8_exception_can_continue_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->CanContinue());
}

static VALUE rb_v8_exception_message(VALUE self)
{
  HandleScope scope;
  return to_ruby(exception(self)->Get(String::New("message")));
}

/* Public constructors */

VALUE rb_v8_exception_new2(TryCatch *try_catch)
{
  HandleScope scope;
  
  //Handle<Value> constructor = Object::Cast(*try_catch->Exception())->Get(String::New("constructor"));
  //VALUE klass = rb_hash_aref(error_types, rb_funcall2(to_ruby(constructor), rb_intern("to_s"), 0, NULL));
  VALUE klass;
  
  v8_try_catch_ref *r = new v8_try_catch_ref(try_catch);
  return Data_Wrap_Struct(rb_eV8Exception, 0, gc_v8_try_catch_free, r);
}


/* V8 Exception types initializer. */
void Init_V8_Exceptions()
{
  rb_eV8Exception = rb_define_class_under(rb_mV8, "Exception", rb_eException);
  rb_define_method(rb_eV8Exception, "can_continue?", RUBY_METHOD_FUNC(rb_v8_exception_can_continue_p), 0);
  rb_define_method(rb_eV8Exception, "message", RUBY_METHOD_FUNC(rb_v8_exception_message), 0);
  rb_define_method(rb_eV8Exception, "to_s", RUBY_METHOD_FUNC(rb_v8_exception_message), 0);
  
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_eV8Exception);
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);

  rb_hash_aset(error_types, rb_str_new2("Exception"), rb_eV8Exception);
  rb_hash_aset(error_types, rb_str_new2("Error"), rb_eV8Error);
  rb_hash_aset(error_types, rb_str_new2("ReferenceError"), rb_eV8ReferenceError);
  rb_hash_aset(error_types, rb_str_new2("RangeError"), rb_eV8RangeError);
  rb_hash_aset(error_types, rb_str_new2("SyntaxError"), rb_eV8SyntaxError);
}
