#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_base.h"
#include "v8_errors.h"
#include "v8_macros.h"

#include <string.h>

using namespace v8;

VALUE rb_eV8Error;
VALUE rb_eV8RangeError;
VALUE rb_eV8ReferenceError;
VALUE rb_eV8SyntaxError;
VALUE rb_eV8TypeError;

/* V8::Error methods */

/*
 * call-seq:
 *   err.syntax_error?  => true or false
 *
 * Returns <code>true</code> when it represents an syntax error.
 *
 */
static VALUE rb_v8_error_syntax_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8SyntaxError);
}

/*
 * call-seq:
 *   err.range_error?  => true or false
 *
 * Returns <code>true</code> when it represents an range error.
 *
 */
static VALUE rb_v8_error_range_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8RangeError);
}

/*
 * call-seq:
 *   err.reference_error?  => true or false
 *
 * Returns <code>true</code> when it represents an reference error.
 *
 */
static VALUE rb_v8_error_reference_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8ReferenceError);
}

/*
 * call-seq:
 *   err.type_error?  => true or false
 *
 * Returns <code>true</code> when it represents an type error.
 *
 */
static VALUE rb_v8_error_type_error_p(VALUE self)
{
  return rb_obj_is_kind_of(self, rb_eV8TypeError);
}

/* Local helpers */

VALUE rb_v8_error_message(Handle<Object> ex)
{
  HandleScope scope;
  String::AsciiValue error_msg(ex->Get(String::New("message")));
  return rb_str_new2(*error_msg);
}

VALUE rb_v8_error_klass(Handle<Object> ex)
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
    } else if (strcmp(type, "TypeError") == 0) {
      return rb_eV8TypeError;
    }
  }

  return rb_eV8Error;
}

/* Public constructors */

VALUE rb_v8_error_new2(Handle<Value> ex, Handle<Message> msg, Handle<Value> trace)
{
  HandleScope scope;
  
  Handle<Object> exc(Object::Cast(*ex));
  VALUE err = rb_funcall2(rb_v8_error_klass(exc), rb_intern("new"), 0, NULL);

  // Error class doesn't have any reflections to v8 objects, so we have to
  // rewrite everything into its instance variables.
  rb_iv_set(err, "@message", rb_v8_error_message(exc));
  rb_iv_set(err, "@line_no", to_ruby(msg->GetLineNumber()));
  rb_iv_set(err, "@source_line", to_ruby(msg->GetSourceLine()));
  rb_iv_set(err, "@script_name", to_ruby(msg->GetScriptResourceName()));
  rb_iv_set(err, "@start_col", to_ruby(msg->GetStartColumn()));
  rb_iv_set(err, "@end_col", to_ruby(msg->GetEndColumn()));
  
  return err;
}

VALUE rb_v8_error_new3(TryCatch try_catch)
{
  return rb_v8_error_new2(try_catch.Exception(), try_catch.Message(), try_catch.StackTrace());
}


/* V8 error types initializer. */
void Init_V8_Errors()
{
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_cV8Data);
  rb_define_method(rb_eV8Error, "reference_error?", RUBY_METHOD_FUNC(rb_v8_error_reference_error_p), 0);
  rb_define_method(rb_eV8Error, "syntax_error?", RUBY_METHOD_FUNC(rb_v8_error_syntax_error_p), 0);
  rb_define_method(rb_eV8Error, "range_error?", RUBY_METHOD_FUNC(rb_v8_error_range_error_p), 0);
  rb_define_method(rb_eV8Error, "type_error?", RUBY_METHOD_FUNC(rb_v8_error_type_error_p), 0);
  rb_define_attr(rb_eV8Error, "message", 1, 0);
  rb_define_attr(rb_eV8Error, "line_no", 1, 0);
  rb_define_attr(rb_eV8Error, "source_line", 1, 0);
  rb_define_attr(rb_eV8Error, "script_name", 1, 0);
  rb_define_attr(rb_eV8Error, "start_col", 1, 0);
  rb_define_attr(rb_eV8Error, "end_col", 1, 0);
  rb_define_attr(rb_eV8Error, "stack_trace", 1, 0);
    
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);
  rb_eV8TypeError = rb_define_class_under(rb_mV8, "TypeError", rb_eV8Error);
}
