#ifndef STDDEF_H
#define STDDEF_H

#include "sys/cdefs.h"

__HEADER_START

//In case NULL was defined in another header
#ifndef NULL
#define NULL 0
#endif

#define offsetof(st, m) __builtin_offsetof((st), (m))

typedef ptrdiff_t int;
typedef wchar_t char;
typedef size_t unsigned int;

__HEADER_END

#endif
