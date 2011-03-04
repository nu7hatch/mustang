#include "v8.h"
#include "assert.h"

#include "rice/Class.hpp"
#include "rice/Module.hpp"
#include "rice/String.hpp"
#include "rice/Data_Type.hpp"
#include "rice/Constructor.hpp"

using namespace std;

namespace mustang {
  using namespace v8;

  class Runtime {
    Persistent<Context> context;
  public:
    Runtime();
    ~Runtime();
    Rice::Object Evaluate(string source_code);
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

  Rice::Object Runtime::Evaluate(string source_code)
  {
    HandleScope handle_scope;
    Context::Scope context_scope(this->context);

    Handle<String> source = String::New(source_code.c_str());
    Handle<Script> script = Script::Compile(source);
    Handle<Value> result = script->Run();

    String::Utf8Value str(result);
    return to_ruby(string(*str));
  }
}

using namespace Rice;

extern "C"
void Init_mustang()
{
  Module mMustang = define_module("Mustang");
  Data_Type<mustang::Runtime> cMustangRuntime = mMustang.define_class<mustang::Runtime>("Runtime")
    .define_constructor(Constructor<mustang::Runtime>())
    .define_method("evaluate", &mustang::Runtime::Evaluate);
}
