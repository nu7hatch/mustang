#include "v8_main.h"
#include "v8_cast.h"
#include "v8_base.h"
#include "v8_context.h"
#include "v8_value.h"
#include "v8_object.h"
#include "v8_string.h"
#include "v8_integer.h"
#include "v8_number.h"
#include "v8_date.h"
#include "v8_array.h"
#include "v8_function.h"
#include "v8_regexp.h"
#include "v8_external.h"
#include "v8_exceptions.h"

extern "C" void Init_v8() {
  Init_V8();
  Init_V8_Cast();
  Init_V8_Data();
  Init_V8_Empty();
  Init_V8_Undefined();
  Init_V8_Null();
  Init_V8_Context();
  Init_V8_Value();
  Init_V8_Primitive();
  Init_V8_Object();
  Init_V8_String();
  Init_V8_Integer();
  Init_V8_Number();
  Init_V8_Date();
  Init_V8_Array();
  Init_V8_Function();
  Init_V8_Regexp();
  Init_V8_External();
  Init_V8_Exceptions();
}
