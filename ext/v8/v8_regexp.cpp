#include "v8_ref.h"
#include "v8_cast.h"
#include "v8_value.h"
#include "v8_regexp.h"
#include "v8_macros.h"

using namespace v8;

VALUE rb_cV8Regexp;
UNWRAPPER(RegExp);

/* Typecasting */

RegExp::Flags to_v8_regexp_flags(VALUE value)
{
  int opts = FIX2INT(value);
  int flags = 0;
  
  if (opts & 1) { // Regexp::IGNORECASE
    flags |= RegExp::kIgnoreCase;
  }
  if (opts & 2) { // Regexp::EXTENDED
    flags |= RegExp::kGlobal;
  }
  if (opts & 4) { // Regexp::MULTILINE
    flags |= RegExp::kMultiline;
  }

  return RegExp::Flags(flags);
}

Handle<Value> to_v8_regexp(VALUE src, VALUE opts)
{
  return RegExp::New(String::New(StringValuePtr(src)), to_v8_regexp_flags(opts));
}

Handle<Value> to_v8_regexp(VALUE value)
{
  if (rb_obj_is_kind_of(value, rb_cRegexp)) {
    VALUE src = rb_funcall2(value, rb_intern("source"), 0, NULL);
    VALUE opts = rb_funcall2(value, rb_intern("options"), 0, NULL);
    return to_v8_regexp(src, opts);
  } else if (rb_obj_is_kind_of(value, rb_cString)) {
    return to_v8_regexp(value, INT2FIX(0));
  } else {
    rb_raise(rb_eArgError, "wrong source type, expected Regexp, got %s", RSTRING_PTR(rb_inspect(value)));
    return Null();
  }
}

/* V8::Regexp methods */

/*
 * call-seq:
 *   V8::Regexp.new(rxp)         => new_regexp
 *   V8::Regexp.new(str, flags)  => new_regexp
 *
 * Returns new V8 regexp reflected from given ruby regexp or source and flags.
 *
 */
static VALUE rb_v8_regexp_new(int argc, VALUE *argv, VALUE klass)
{
  HandleScope scope;

  if (argc == 1) {
    return v8_ref_new(klass, to_v8_regexp(argv[0]));
  } else if (argc == 2) {
    return v8_ref_new(klass, to_v8_regexp(argv[0], argv[1]));
  } else {
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..2)", argc);
    return Qnil;
  }
  
}

/*
 * call-seq:
 *   rxp.source  => str
 *
 * Returns regexp source string.
 *
 *   rxp = V8::Regexp.new(/foo(bar)?/i)
 *   rxp.source # => "foo(bar)?"
 * 
 */
static VALUE rb_v8_regexp_source(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->GetSource());
}

/*
 * call-seq:
 *   rxp.options  => int
 *
 * Returns fixnum value of v8 regexp flags.
 *
 */
static VALUE rb_v8_regexp_flags(VALUE self)
{
  HandleScope scope;
  return to_ruby(unwrap(self)->GetFlags());
}

/*
 * call-seq:
 *   rxp.flags  => int
 *
 * Returns fixnum value of ruby regexp flags.
 *
 */
static VALUE rb_v8_regexp_options(VALUE self)
{
  HandleScope scope;
  int flags = (int)unwrap(self)->GetFlags();
  int opts = 0;

  if (flags & RegExp::kIgnoreCase) {
    opts |= 1; // Regexp::IGNORECASE
  }
  if (flags & RegExp::kGlobal) {
    opts |= 2; // Regexp::EXTENDED
  }
  if (flags & RegExp::kMultiline) {
    opts |= 4; // Regexp::MULTILINE
  }
  
  return to_ruby(opts);
}


/* Public constructors */

VALUE rb_v8_regexp_new2(VALUE data)
{
  return rb_v8_regexp_new(1, &data, rb_cV8Regexp);
}


/* V8::Regexp initializer. */
void Init_V8_Regexp()
{
  rb_cV8Regexp = rb_define_class_under(rb_mV8, "Regexp", rb_cV8Value);
  rb_define_singleton_method(rb_cV8Regexp, "new", RUBY_METHOD_FUNC(rb_v8_regexp_new), -1);
  rb_define_method(rb_cV8Regexp, "source", RUBY_METHOD_FUNC(rb_v8_regexp_source), 0);
  rb_define_method(rb_cV8Regexp, "flags", RUBY_METHOD_FUNC(rb_v8_regexp_flags), 0);
  rb_define_method(rb_cV8Regexp, "options", RUBY_METHOD_FUNC(rb_v8_regexp_options), 0);
}
