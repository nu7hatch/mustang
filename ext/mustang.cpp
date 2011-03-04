#include "v8.h"
#include "string.h"
#include "assert.h"

#include "rice/Class.hpp"
#include "rice/Module.hpp"
#include "rice/String.hpp"
#include "rice/Data_Type.hpp"

using namespace std;
using namespace Rice;

Object Mustang_evaluate(string source_code)
{
  v8::HandleScope handle_scope;
  v8::Persistent<v8::Context> context = v8::Context::New();
  v8::Context::Scope context_scope(context);

  v8::Handle<v8::String> source = v8::String::New(source_code.c_str());
  v8::Handle<v8::Script> script = v8::Script::Compile(source);
  v8::Handle<v8::Value> result = script->Run();

  context.Dispose();

  v8::String::AsciiValue str(result);
  return to_ruby(string(*str));
}

extern "C"
void Init_mustang()
{
  Module mMustang = define_module("Mustang");
  mMustang.define_method("evaluate", &Mustang_evaluate);
}
