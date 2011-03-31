#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_exceptions.h"
#include "v8_macros.h"

#include <string.h>

using namespace v8;

VALUE rb_eV8Exception;
VALUE rb_eV8Error;
VALUE rb_eV8RangeError;
VALUE rb_eV8ReferenceError;
VALUE rb_eV8SyntaxError;

/* V8::Exception methods */

/*
 * call-seq:
 *   exc.error?  => true
 *
 * Always returns true.
 *
 */
static VALUE rb_v8_exception_error_p(VALUE self)
{
  return true;
}

/*
 * call-seq:
 *   exc.syntax_error?  => true or false
 *
 * Returns <code>true</code> when it represents an syntax error.
 *
 */
static VALUE rb_v8_exception_syntax_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8SyntaxError);
}

/*
 * call-seq:
 *   exc.range_error?  => true or false
 *
 * Returns <code>true</code> when it represents an range error.
 *
 */
static VALUE rb_v8_exception_range_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8RangeError);
}

/*
 * call-seq:
 *   exc.reference_error?  => true or false
 *
 * Returns <code>true</code> when it represents an reference error.
 *
 */
static VALUE rb_v8_exception_reference_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8ReferenceError);
}

/* Local helpers */

VALUE rb_v8_exception_message(Handle<Object> ex)
{
  HandleScope scope;
  String::AsciiValue error_msg(ex->Get(String::New("message")));
  return rb_str_new2(*error_msg);
}

VALUE rb_v8_exception_type(Handle<Object> ex)
{
  HandleScope scope;
  Handle<Function> con(Function::Cast(*ex->Get(String::New("constructor"))));

  if (!con.IsEmpty() && !con->IsUndefined() && !con->IsNull()) {
    String::AsciiValue con_name(con->GetName());
    char* type = (char*)*con_name;

    if (strcmp(type, "SyntaxError") == 0) {
      return rb_eV8SyntaxError;
    } else if (strcmp(type, "ReferenceError") == 0) {
      return rb_eV8ReferenceError;
    } else if (strcmp(type, "RangeError") == 0) {
      return rb_eV8RangeError;
    }
  }

  return rb_eV8Error;
}

/* Public constructors */

VALUE rb_v8_exception_new2(Handle<Value> ex, Handle<Message> msg)
{
  HandleScope scope;
  Handle<Object> exo(Object::Cast(*ex));
  VALUE exc = rb_exc_new3(rb_v8_exception_type(exo), rb_v8_exception_message(exo));

  rb_iv_set(exc, "@line_no", to_ruby(msg->GetLineNumber()));
  rb_iv_set(exc, "@source_line", to_ruby(msg->GetSourceLine()));
  rb_iv_set(exc, "@script_name", to_ruby(msg->GetScriptResourceName()));
  rb_iv_set(exc, "@start_col", to_ruby(msg->GetStartColumn()));
  rb_iv_set(exc, "@end_col", to_ruby(msg->GetEndColumn()));

  return exc;
}

VALUE rb_v8_exception_new3(TryCatch try_catch)
{
  return rb_v8_exception_new2(try_catch.Exception(), try_catch.Message());
}


/* V8 Exception types initializer. */
void Init_V8_Exceptions()
{
  rb_eV8Exception = rb_define_class_under(rb_mV8, "Exception", rb_eException);
  rb_define_method(rb_eV8Exception, "reference_error?", RUBY_METHOD_FUNC(rb_v8_exception_reference_error_p), 0);
  rb_define_method(rb_eV8Exception, "syntax_error?", RUBY_METHOD_FUNC(rb_v8_exception_syntax_error_p), 0);
  rb_define_method(rb_eV8Exception, "range_error?", RUBY_METHOD_FUNC(rb_v8_exception_range_error_p), 0);
  rb_define_method(rb_eV8Exception, "error?", RUBY_METHOD_FUNC(rb_v8_exception_error_p), 0);
  rb_define_attr(rb_eV8Exception, "line_no", 1, 0);
  rb_define_attr(rb_eV8Exception, "source_line", 1, 0);
  rb_define_attr(rb_eV8Exception, "script_name", 1, 0);
  rb_define_attr(rb_eV8Exception, "start_col", 1, 0);
  rb_define_attr(rb_eV8Exception, "end_col", 1, 0);
    
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_eV8Exception);
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);
}
