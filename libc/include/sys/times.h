#ifndef TIMES_H
#define TIMES_H

#include "sys/cdefs.h"

__HEADER_START

struct tms{
	clock_t tms_utime;
	clock_t tms_stime;
	clock_t tms_cutime;
	clock_t tms_cstime;
};

clock_t times(struct tms* buf);

__HEADER_END

#endif
