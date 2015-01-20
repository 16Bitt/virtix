#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "sys/cdefs.h"

__HEADER_START

#define SYS_GETPID	0
#define SYS_FORK	1
#define SYS_EXEC	2
#define SYS_OPEN	3
#define SYS_WRITE	4
#define SYS_READ	5

int syscall_0arg(uint call_no);
int syscall_1arg(uint call_no, uint arg1);
int syscall_2arg(uint call_no, uint arg1, uint arg2);
int syscall_3arg(uint call_no, uint arg1, uint arg2, uint arg3);

__HEADER_END

#endif
