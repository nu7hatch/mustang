#ifndef __V8_MACROS_H
#define __V8_MACROS_H

#ifndef rb_sym_to_s
#define rb_sym_to_s(sym) rb_funcall2(sym, rb_intern("to_s"), 0, NULL)
#endif

#endif//__V8_MACROS_H
