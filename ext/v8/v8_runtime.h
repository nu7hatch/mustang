#ifndef __V8_RUNTIME_H
#define __V8_RUNTIME_H

#include <v8.h>
#include <ruby.h>
#include <string>

using namespace v8;

#define runtimeDefineScopes()					\
  HandleScope scope;						\
  Context::Scope context_scope(runtimeGetContext(self));

/* This struct wraps Mustang::V8::Runtime class for ruby's garbage
   collector. */
struct runtime {
  Persistent<Context> context;
  // TODO: add v8 handles references...
};

/* API */
void initRuntime(VALUE parent);

#endif//__V8_RUNTIME_H
