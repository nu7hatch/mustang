#include "engine.hpp"
#include "runtime.hpp"

namespace engine
{
  Runtime::Runtime()
  {
    HandleScope handle_scope;
    Handle<ObjectTemplate> global;

    { // Initialize global objects
      global = ObjectTemplate::New();

      // Mustang
      Local<ObjectTemplate> env = ObjectTemplate::New();
      env->Set(String::NewSymbol("version"), String::New(ENGINE_VERSION));
      global->Set(String::NewSymbol("Mustang"), env);
    
      // Mustang.V8
      Local<ObjectTemplate> env_v8 = ObjectTemplate::New();
      env_v8->Set(String::NewSymbol("version"), String::New(V8::GetVersion()));
      env->Set(String::NewSymbol("V8"), env_v8);      
    } 

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

    Local<String> source = String::New(source_code.c_str());
    Local<String> filename = String::New(source_name.c_str());

    Handle<Script> script = Script::Compile(source, filename);
    Handle<Value> result = script->Run();

    String::Utf8Value str(result);
    return to_ruby(string(*str));
  }
}
