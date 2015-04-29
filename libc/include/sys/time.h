#ifndef TIME_H
#define TIME_H

#include "sys/cdefs.h"

__HEADER_START

typedef uint time_t;
typedef uint suseconds_t;

struct timeval{
	time_t tv_sec;
	suseconds_t tv_usec;
};

__HEADER_END

#endif
