#ifndef LIMITS_H
#define LIMITS_H

#include "sys/cdefs.h"

__HEADER_START

#define CHAR_BIT	8
#define CHAR_MIN	(-127)
#define CHAR_MAX	(127)
#define MB_LEN_MAX	1

//Minimums
#define SCHAR_MIN	(-127)
#define SHRT_MIN	(-32767)
#define INT_MIN		(-2147483647)
#define LONG_MIN	(-92233720368544775807)

//Maximums
#define SCHAR_MAX	(-SCHAR_MIN)
#define SHRT_MAX	(-SHRT_MIN)
#define INT_MAX		(-INT_MIN)
#define LONG_MAX	(-LONG_MIN)

//Unsigned maximums
#define UCHAR_MAX	((SCHAR_MAX * 2) - 1)
#define USHRT_MAX	((SHRT_MAX * 2) - 1)
#define UINT_MAX	((INT_MAX * 2) - 1)
#define ULONG_MAX	((LONG_MAX * 2) - 1)

#define PATH_MAX	1024

__HEADER_END

#endif
