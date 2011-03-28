#include "v8_main.h"
#include "v8_context.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_array.h"
#include "v8_function.h"
#include "v8_external.h"

extern "C" void Init_v8() {
  Init_V8();
  Init_V8_Context();
  Init_V8_Object();
  Init_V8_String();
  Init_V8_Integer();
  Init_V8_Array();
  Init_V8_Function();
  Init_V8_External();
}
