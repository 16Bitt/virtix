#ifndef RESOURCE_H
#define RESOURCE_H

#include "sys/cdefs.h"

__HEADER_START

#include "sys/time.h"

typedef uint rlim_t;

#define PRIO_PROCESS	1
#define PRIO_PGRP	2
#define PRIO_USER	3

#define RLIM_INFINITY

#define RUSAGE_SELF	1
#define RUSAGE_CHILDREN	2

struct rlimit{
	rlim_t	rlim_cur;
	rlim_t	rlim_max;
};

struct rusage{
	struct timeval ru_utime;
	struct timeval ru_stime;
};

__HEADER_END

#endif
