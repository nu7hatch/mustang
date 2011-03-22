#ifndef __V8_OBJECTS_H
#define __V8_OBJECTS_H

#include <v8.h>
#include <ruby.h>
#include <string>

using namespace v8;

#define cstr2v8(cstr)				\
  String::New(cstr)

#define rbstr2v8(rbstr)				\
  cstr2v8(StringValuePtr(rbstr))

/* API */
Handle<Value> rb2handle(VALUE value);
VALUE handle2rb(Handle<Value> value);
VALUE handle2rb(Handle<Boolean> value);
VALUE handle2rb(Handle<Number> value);
VALUE handle2rb(Handle<String> value);
VALUE handle2rb(Handle<Object> value);
VALUE handle2rb(Handle<Array> value);
VALUE handle2rb(Handle<Function> value);
VALUE handle2rb(Handle<Integer> value);
VALUE handle2rb(Handle<Uint32> value);
VALUE handle2rb(Handle<Int32> value);
VALUE handle2rb(bool value);
VALUE handle2rb(double value);
VALUE handle2rb(int64_t value);
VALUE handle2rb(uint32_t value);
VALUE handle2rb(int32_t value);

#endif//__V8_OBJECTS_H
