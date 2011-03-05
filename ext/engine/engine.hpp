#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "v8.h"

#include "rice/Class.hpp"
#include "rice/Module.hpp"
#include "rice/String.hpp"
#include "rice/Data_Type.hpp"
#include "rice/Constructor.hpp"

#define ENGINE_VERSION "0.0.2"

namespace engine
{
  using namespace std;
  using namespace v8;
  
  static Rice::Object GetVersion()
  {
    return to_ruby(string(ENGINE_VERSION));
  }
};

#endif // __ENGINE_HPP
