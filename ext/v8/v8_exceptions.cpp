#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_exceptions.h"

#include <string.h>

using namespace v8;

VALUE rb_eV8Exception;
VALUE rb_eV8Error;
VALUE rb_eV8RangeError;
VALUE rb_eV8ReferenceError;
VALUE rb_eV8SyntaxError;

/* V8 TryCatch reference */

struct v8_try_catch_ref {
  v8_try_catch_ref(TryCatch *tc);
  virtual ~v8_try_catch_ref();
  TryCatch *try_catch;
  bool alive;
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

  //if (r->alive) {
    return r->try_catch;
    //} else {
    //rb_raise(rb_eScriptError, "out of scope access of %s", RSTRING_PTR(rb_inspect(self)));
    //return NULL;
    //}
}

Handle<Object> exception(VALUE self)
{
  HandleScope scope;
  return (Handle<Object>)Object::Cast(*unwrap(self)->Exception());
}

/* V8::Exception methods */

static VALUE rb_v8_exception_can_continue_p(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->CanContinue());
}

static VALUE rb_v8_exception_exception(VALUE self)
{
  HandleScope scope;
  Handle<Object> ex(Object::Cast(*unwrap(self)->Exception()));
  return to_ruby(ex->Get(String::New("message")));
}

static VALUE rb_v8_exception_syntax_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8SyntaxError);
}

static VALUE rb_v8_exception_range_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8RangeError);
}

static VALUE rb_v8_exception_reference_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8ReferenceError);
}

/* Public constructors */

VALUE rb_v8_exception_new2(TryCatch *try_catch)
{
  //HandleScope scope;

  /*
  Handle<Object> ex(Object::Cast(*try_catch->Exception()));
  Handle<Function> con(Function::Cast(*ex->Get(String::New("constructor"))));
  
  String::AsciiValue cname(con->GetName());
  char* klass_name = (char*)*cname;
  VALUE klass = Qnil;

  if (strcmp(klass_name, "ReferenceError") == 0) {
    klass = rb_eV8ReferenceError;
  } else if (strcmp(klass_name, "SyntaxError") == 0) {
    klass = rb_eV8SyntaxError;
  } else if (strcmp(klass_name, "RangeError") == 0) {
    klass = rb_eV8RangeError;
  } else {
    klass = rb_eV8Error;
  }
  */
  
  v8_try_catch_ref *r = new v8_try_catch_ref(try_catch);
  return Data_Wrap_Struct(rb_eV8Exception, 0, gc_v8_try_catch_free, r);
}


/* V8 Exception types initializer. */
void Init_V8_Exceptions()
{
  rb_eV8Exception = rb_define_class_under(rb_mV8, "Exception", rb_eException);
  rb_define_method(rb_eV8Exception, "can_continue?", RUBY_METHOD_FUNC(rb_v8_exception_can_continue_p), 0);
  rb_define_method(rb_eV8Exception, "exception", RUBY_METHOD_FUNC(rb_v8_exception_exception), 0);
  //rb_define_method(rb_eV8Exception, "to_s", RUBY_METHOD_FUNC(rb_v8_exception_message), 0);
  rb_define_method(rb_eV8Exception, "reference_error?", RUBY_METHOD_FUNC(rb_v8_exception_reference_error_p), 0);
  rb_define_method(rb_eV8Exception, "syntax_error?", RUBY_METHOD_FUNC(rb_v8_exception_syntax_error_p), 0);
  rb_define_method(rb_eV8Exception, "range_error?", RUBY_METHOD_FUNC(rb_v8_exception_range_error_p), 0);
  
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_eV8Exception);
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);
}
