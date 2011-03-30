#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_exceptions.h"
#include "v8_macros.h"

#include <string.h>

using namespace v8;

VALUE rb_cV8TryCatch;
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

namespace
{
  TryCatch* unwrap_tc(VALUE self)
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

  /* V8::TryCatch methods */

  static VALUE rb_v8_try_catch_can_continue_p(VALUE self)
  {
    HandleScope scope;
    return to_ruby(unwrap_tc(self)->CanContinue());
  }
}

namespace
{
  UNWRAPPER(Object);

  Handle<Message> exception_info(VALUE self)
  {
    HandleScope scope;
    Handle<External> info(External::Cast(*unwrap(self)->Get(String::New("info"))));
    return Handle<Message>((Message*)info->Value());
  }
  
  static VALUE rb_v8_exception_message(VALUE self)
  {
    HandleScope scope;
    VALUE msg = rb_iv_get(self, "@message");

    if (NIL_P(msg)) {
      return to_ruby(unwrap(self)->Get(String::New("message")));
    } else {
      return msg;
    }
  }

  static VALUE rb_v8_exception_line_no(VALUE self)
  {
    HandleScope scope;
    return to_ruby(exception_info(self)->GetLineNumber());
  }

  static VALUE rb_v8_exception_script_name(VALUE self)
  {
    HandleScope scope;
    return to_ruby(exception_info(self)->GetScriptResourceName());
  }

  static VALUE rb_v8_exception_source_line(VALUE self)
  {
    HandleScope scope;
    return to_ruby(exception_info(self)->GetSourceLine());
  }

  static VALUE rb_v8_exception_column_start_no(VALUE self)
  {
    HandleScope scope;
    return to_ruby(exception_info(self)->GetStartColumn());
  }

  static VALUE rb_v8_exception_column_end_no(VALUE self)
  {
    HandleScope scope;
    return to_ruby(exception_info(self)->GetStartColumn());
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

  static VALUE rb_v8_exception_exception(int argc, VALUE *argv, VALUE self)
  {
    if (argc = 1) {
      rb_iv_set(self, "@msg", argv[1]);
    } else if (argc > 1) {
      rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }
  
    return self;
  }
}

/* Public constructors */

VALUE rb_v8_try_catch_as_exception(TryCatch *try_catch)
{
  Handle<Object> ex(Object::Cast(*try_catch->Exception()));
  ex->Set(String::New("info"), External::New(*try_catch->Message()));
  //ex->Set(String::New("stack_trace"), try_catch->StackTrace());

  return rb_v8_exception_new2(ex);
}

char* v8_exception_get_type_name(Handle<Object> ex)
{
  HandleScope scope;
  Handle<Function> con(Function::Cast(*ex->Get(String::New("constructor"))));
  
  String::AsciiValue error_name(con->GetName());
  char* name = (char*)*error_name;

  return name;
}

VALUE rb_v8_exception_new2(Handle<Value> value)
{
  HandleScope scope;
  
  Handle<Object> ex(Object::Cast(*value));
  char* klass_name = v8_exception_get_type_name(ex);
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

  return v8_ref_new(klass, ex);
}


/* V8 Exception types initializer. */
void Init_V8_Exceptions()
{
  rb_cV8TryCatch = rb_define_class_under(rb_mV8, "TryCatch", rb_cObject);
  rb_define_method(rb_cV8TryCatch, "can_continue?", RUBY_METHOD_FUNC(rb_v8_try_catch_can_continue_p), 0);
  
  rb_eV8Exception = rb_define_class_under(rb_mV8, "Exception", rb_eException);
  rb_define_method(rb_eV8Exception, "exception", RUBY_METHOD_FUNC(rb_v8_exception_exception), -1);
  rb_define_method(rb_eV8Exception, "reference_error?", RUBY_METHOD_FUNC(rb_v8_exception_reference_error_p), 0);
  rb_define_method(rb_eV8Exception, "syntax_error?", RUBY_METHOD_FUNC(rb_v8_exception_syntax_error_p), 0);
  rb_define_method(rb_eV8Exception, "range_error?", RUBY_METHOD_FUNC(rb_v8_exception_range_error_p), 0);
  rb_define_method(rb_eV8Exception, "message", RUBY_METHOD_FUNC(rb_v8_exception_message), 0);
  rb_define_method(rb_eV8Exception, "to_s", RUBY_METHOD_FUNC(rb_v8_exception_message), 0);
  rb_define_method(rb_eV8Exception, "line_no", RUBY_METHOD_FUNC(rb_v8_exception_line_no), 0);
  rb_define_method(rb_eV8Exception, "script_name", RUBY_METHOD_FUNC(rb_v8_exception_script_name), 0);
  rb_define_method(rb_eV8Exception, "source_line", RUBY_METHOD_FUNC(rb_v8_exception_source_line), 0);
  rb_define_method(rb_eV8Exception, "column_start_no", RUBY_METHOD_FUNC(rb_v8_exception_column_start_no), 0);
  rb_define_method(rb_eV8Exception, "column_end_no", RUBY_METHOD_FUNC(rb_v8_exception_column_end_no), 0);
 
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_eV8Exception);
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);
}
