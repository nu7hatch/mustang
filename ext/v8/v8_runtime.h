#ifndef __V8_RUNTIME_H
#define __V8_RUNTIME_H

#include <v8.h>
#include <ruby.h>
#include <string>

#define cstr2v8(cstr) String::New(cstr)
#define rbstr2v8(rbstr) cstr2v8(STR2CSTR(rbstr))

using namespace v8;

struct runtime {
  Persistent<Context> context;
  // TODO: add v8 handles references...
};

/* API */
void initRuntime(VALUE parent);

#endif//__V8_RUNTIME_H
