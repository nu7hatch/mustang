#include "engine.hpp"
#include "runtime.hpp"

using namespace Rice;

extern "C"
void Init_engine()
{
  // Mustang module
  Module mMustang = define_module("Mustang")
    .define_singleton_method("version", &engine::GetVersion);

  // Mustang::Engine module
  Module mMustangEngine = mMustang.define_module("Engine");

  // Mustang::Engine::Runtime class
  Data_Type<engine::Runtime> cMustangEngineRuntime = mMustangEngine.define_class<engine::Runtime>("Runtime")
    .define_constructor(Constructor<engine::Runtime>())
    .define_method("evaluate", &engine::Runtime::Evaluate);
}
