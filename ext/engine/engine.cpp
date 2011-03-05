#include "v8.h"

#include "rice/Class.hpp"
#include "rice/Module.hpp"
#include "rice/String.hpp"
#include "rice/Data_Type.hpp"
#include "rice/Constructor.hpp"

using namespace std;

namespace engine {
  using namespace v8;

  class Runtime {
    Persistent<Context> context;
  public:
    Runtime();
    ~Runtime();
    Rice::Object Evaluate(string source_code, string source_name);
  };

  Runtime::Runtime()
  {
    HandleScope handle_scope;
    Handle<ObjectTemplate> global = ObjectTemplate::New();
    this->context = Context::New(NULL, global);
  }

  Runtime::~Runtime()
  {
    this->context.Dispose();
  }

  Rice::Object Runtime::Evaluate(string source_code, string source_name)
  {
    HandleScope handle_scope;
    Context::Scope context_scope(this->context);

    Handle<String> source = String::New(source_code.c_str());
    Handle<String> filename = String::New(source_name.c_str());

    Handle<Script> script = Script::Compile(source, filename);
    Handle<Value> result = script->Run();

    String::Utf8Value str(result);
    return to_ruby(string(*str));
  }
}

using namespace Rice;

extern "C"
void Init_engine()
{
  // module Mustang
  Module mMustang = define_module("Mustang");
  // module Mustang::Engine
  Module mMustangEngine = mMustang.define_module("Engine");
  // class Mustang::Engine::Runtime
  Data_Type<engine::Runtime> cMustangEngineRuntime = mMustangEngine.define_class<engine::Runtime>("Runtime")
    .define_constructor(Constructor<engine::Runtime>())
    .define_method("evaluate", &engine::Runtime::Evaluate);
}
