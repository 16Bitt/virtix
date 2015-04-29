#ifndef FCNTL_H
#define FCNTL_H

#include "sys/cdefs.h"

__HEADER_START

#define BUFSIZ	512

#define F_GETFL	1
#define F_SETFL	2
#define F_SETFD	3
#define F_GETFD	4
#define F_DUPFD	5

#define FD_CLOEXEC	1

#define O_NONBLOCK	1
#define O_RDONLY	2
#define O_RDWR		4
#define O_CREAT		8
#define O_TRUNC		16
#define O_WRONLY	32
#define O_EXCL		64
#define O_APPEND	128

int open(const char* pathname, int flags, ...);

__HEADER_END

#endif
