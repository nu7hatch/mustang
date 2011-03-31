#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_base.h"
#include "v8_exceptions.h"
#include "v8_macros.h"

#include <string.h>

using namespace v8;

VALUE rb_cV8StackTrace;
VALUE rb_cV8StackFrame;

VALUE rb_eV8Exception;
VALUE rb_eV8Error;
VALUE rb_eV8RangeError;
VALUE rb_eV8ReferenceError;
VALUE rb_eV8SyntaxError;

/* Unwrappers */

Handle<StackTrace> unwrap_trace(VALUE value)
{
  return v8_ref_get<StackTrace>(value);
}

Handle<StackFrame> unwrap_frame(VALUE value)
{
  return v8_ref_get<StackFrame>(value);
}

/* V8::StackTrace methods */

/*
 * call-seq:
 *   st.frame(key)  => stack_frame
 *   st[key]        => stack_frame
 *
 * Returns specified stack frame object.
 *
 */
static VALUE rb_v8_stack_trace_frame(VALUE self, VALUE key)
{
  HandleScope scope;
  Handle<StackTrace> trace = unwrap_trace(self);
  VALUE _key = rb_funcall2(key, rb_intern("to_i"), 0, NULL);
  int frameid = FIX2INT(_key);
  
  if (frameid > trace->GetFrameCount()) {
    return Qnil;
  } else {
    return to_ruby(trace->GetFrame(frameid));
  }
}

/*
 * call-seq:
 *   st.to_a  => array of stack frames
 *
 * Returns array with stack frames.
 *
 */
static VALUE rb_v8_stack_trace_to_a(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap_trace(self)->AsArray());
}

/*
 * call-seq:
 *   st.length  => count
 *   st.size    => count
 *
 * Returns number of stack frames within this trace.
 *
 */
static VALUE rb_v8_stack_trace_length(VALUE self)
{
  HandleScope scope;
  return to_ruby(1);
  return to_ruby(unwrap_trace(self)->GetFrameCount());
}

/* Public constructors */

VALUE rb_v8_stack_trace_new2(Handle<StackTrace> stack_trace)
{
  return v8_ref_new(rb_cV8StackTrace, stack_trace);
}


/* V8::StackFrame methods */

/* Public constructors */

VALUE rb_v8_stack_frame_new2(Handle<StackFrame> stack_frame)
{
  return v8_ref_new(rb_cV8StackFrame, stack_frame);
}


/* V8::Exception methods */

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

VALUE rb_v8_exception_new2(Handle<Value> ex, Handle<Message> msg, Handle<Value> trace)
{
  HandleScope scope;
  Handle<Object> exo(Object::Cast(*ex));
  VALUE exc = rb_exc_new3(rb_v8_exception_type(exo), rb_v8_exception_message(exo));

  rb_iv_set(exc, "@line_no", to_ruby(msg->GetLineNumber()));
  rb_iv_set(exc, "@source_line", to_ruby(msg->GetSourceLine()));
  rb_iv_set(exc, "@script_name", to_ruby(msg->GetScriptResourceName()));
  rb_iv_set(exc, "@start_col", to_ruby(msg->GetStartColumn()));
  rb_iv_set(exc, "@end_col", to_ruby(msg->GetEndColumn()));
  //rb_iv_set(exc, "@stack_trace", to_ruby(msg->GetStackTrace()));
  
  return exc;
}

VALUE rb_v8_exception_new3(TryCatch try_catch)
{
  return rb_v8_exception_new2(try_catch.Exception(), try_catch.Message(), try_catch.StackTrace());
}


/* V8 Exceptions types initializer. */
void Init_V8_Exceptions()
{
  rb_cV8StackTrace = rb_define_class_under(rb_mV8, "StackTrace", rb_cV8Value);
  rb_define_method(rb_cV8StackTrace, "frame", RUBY_METHOD_FUNC(rb_v8_stack_trace_frame), 1);
  rb_define_method(rb_cV8StackTrace, "[]", RUBY_METHOD_FUNC(rb_v8_stack_trace_frame), 1);
  rb_define_method(rb_cV8StackTrace, "to_a", RUBY_METHOD_FUNC(rb_v8_stack_trace_to_a), 0);
  rb_define_method(rb_cV8StackTrace, "length", RUBY_METHOD_FUNC(rb_v8_stack_trace_length), 0);
  rb_define_method(rb_cV8StackTrace, "size", RUBY_METHOD_FUNC(rb_v8_stack_trace_length), 0);
  
  rb_cV8StackFrame = rb_define_class_under(rb_mV8, "StackFrame", rb_cV8Value);
  
  rb_eV8Exception = rb_define_class_under(rb_mV8, "Exception", rb_cV8Data);
  rb_define_method(rb_eV8Exception, "reference_error?", RUBY_METHOD_FUNC(rb_v8_exception_reference_error_p), 0);
  rb_define_method(rb_eV8Exception, "syntax_error?", RUBY_METHOD_FUNC(rb_v8_exception_syntax_error_p), 0);
  rb_define_method(rb_eV8Exception, "range_error?", RUBY_METHOD_FUNC(rb_v8_exception_range_error_p), 0);
  rb_define_attr(rb_eV8Exception, "line_no", 1, 0);
  rb_define_attr(rb_eV8Exception, "source_line", 1, 0);
  rb_define_attr(rb_eV8Exception, "script_name", 1, 0);
  rb_define_attr(rb_eV8Exception, "start_col", 1, 0);
  rb_define_attr(rb_eV8Exception, "end_col", 1, 0);
  rb_define_attr(rb_eV8Exception, "stack_trace", 1, 0);
    
  rb_eV8Error = rb_define_class_under(rb_mV8, "Error", rb_eV8Exception);
  rb_eV8RangeError = rb_define_class_under(rb_mV8, "RangeError", rb_eV8Error);
  rb_eV8ReferenceError = rb_define_class_under(rb_mV8, "ReferenceError", rb_eV8Error);
  rb_eV8SyntaxError = rb_define_class_under(rb_mV8, "SyntaxError", rb_eV8Error);
}
