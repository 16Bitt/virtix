#ifndef FNMATCH_H
#define FNMATCH_H

#include "sys/cdefs.h"

__HEADER_START

int fnmatch(const char* s1, const char* s2, int flags);

#define FNM_NOMATCH	1
#define FNM_PATHNAME	2
#define FNM_PERIOD	4
#define FNM_NOESCAPE	8

__HEADER_END

#endif
