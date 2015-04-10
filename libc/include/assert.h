#ifndef ASSERT_H
#define ASSERT_H

#include "sys/cdefs.h"

__HEADER_START

#include "stdio.h"

#define assert(x)\
	if((x) == 0){\
		printf("Assertion failed in %s on line %d: %s\n", __FILE__, __LINE__, __func__);\
		exit(0);\
	}

__HEADER_END

#endif
