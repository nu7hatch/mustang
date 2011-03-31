#ifndef __V8_CAST_H
#define __V8_CAST_H

#include "v8_main.h"
#include "v8_macros.h"

using namespace v8;

/* V8::Cast module */
RUBY_EXTERN VALUE rb_mV8Cast;

/* API */
Handle<Value> to_v8(VALUE value);

VALUE to_ruby(Handle<Value> value);
VALUE to_ruby(Handle<Boolean> value);
VALUE to_ruby(Handle<String> value);
VALUE to_ruby(Handle<Integer> value);
VALUE to_ruby(Handle<Number> value);
VALUE to_ruby(Handle<Function> value);
VALUE to_ruby(Handle<Array> value);
VALUE to_ruby(Handle<Object> value);
VALUE to_ruby(Handle<StackTrace> value);
VALUE to_ruby(Handle<StackFrame> value);

VALUE to_ruby(bool value);
VALUE to_ruby(double value);
VALUE to_ruby(int64_t value);
VALUE to_ruby(uint32_t value);
VALUE to_ruby(int32_t value);
VALUE to_ruby(char *value);

void Init_V8_Cast();

/* Universal converters */

template <class T> VALUE to_ruby_with_peer(Handle<Value> value, VALUE klass)
{
  HandleScope scope;
  Local<T> obj = T::Cast(*value);
  Local<Value> peer = obj->GetHiddenValue(String::NewSymbol(RUBY_PEER_ATTR));
  return peer.IsEmpty() ? v8_ref_new(klass, obj) : (VALUE)External::Unwrap(peer);
}

template <class T> VALUE to_ruby_without_peer(Handle<Value> value, VALUE klass)
{
  HandleScope scope;
  Local<T> obj = T::Cast(*value);
  return v8_ref_new(klass, obj);
}

template <class T> VALUE to_ruby_no_cast(Handle<T> value, VALUE klass)
{
  HandleScope scope;
  return v8_ref_new(klass, value);
}

#endif//__V8_CAST_H
