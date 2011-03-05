#ifndef __RUNTIME_HPP
#define __RUNTIME_HPP

namespace engine
{
  class Runtime {
    Persistent<Object> env;
    Persistent<Context> context;
  public:
    Runtime();
    ~Runtime();
    Rice::Object Evaluate(string source_code, string source_name);
  };
}

#endif // __RUNTIME_HPP
