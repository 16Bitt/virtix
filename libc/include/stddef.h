#ifndef STDDEF_H
#define STDDEF_H

#include "sys/cdefs.h"

__HEADER_START

//In case NULL was defined in another header
#ifndef NULL
#define NULL 0
#endif

#define offsetof(st, m) __builtin_offsetof((st), (m))

typedef int ptrdiff_t;
typedef char wchar_t;

__HEADER_END

#endif
