#ifndef ERRNO_H
#define ERRNO_H

#include "sys/cdefs.h"

__HEADER_START

extern int errno;

#define EDOM	(-1)
#define ERANGE	(-2)
#define EILSEQ	(-3)
#define ENOENT	(-4)
#define ENOTDIR	(-5)
#define EACCES	(-6)
#define ENOEXEC	(-7)
#define EINTR	(-8)
#define EWOULDBLOCK	(-9)
#define EAGAIN	(-9)
#define EEXIST	(-10)
#define EBADF	(-11)

__HEADER_END

#endif
