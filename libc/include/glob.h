#ifndef GLOB_H
#define GLOB_H

#include "sys/cdefs.h"

__HEADER_START

typedef struct{
	size_t gl_pathc;
	char** gl_pathv;
	size_t gl_offs;
} glob_t;

int glob(const char* pattern, int flags, int (*errfunc)(const char* epath, int eerrno), glob_t* pglob);

//Passed to the flags argument
#define GLOB_APPEND	1	//Start at the end of the current list
#define GLOB_DOOFS	2	//Fill up to the offset with NULL pointers
#define GLOB_ERR	4	//Return on error
#define GLOB_MARK	8	//Directories have slashes
#define GLOB_NOCHECK	16	//If the pattern doesn't match, just return the pattern
#define GLOB_NOESCAPE	32	//Ignore backslash escapes
#define GLOB_NOSORT	64

//Possible return values
#define GLOB_ABORTED	(-1)
#define GLOB_NOMATCH	(-2)
#define GLOB_NOSPACE	(-3)
#define GLOB_NOSYS	(-4)	//Reserved according to docs?

__HEADER_END

#endif
