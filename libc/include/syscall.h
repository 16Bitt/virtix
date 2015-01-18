#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "sys/cdefs.h"

__HEADER_START

int syscall_0arg(uint call_no);
int syscall_1arg(uint call_no, uint arg1);
int syscall_2arg(uint call_no, uint arg1, uint arg2);
int syscall_3arg(uint call_no, uint arg1, uint arg2, uint arg3);

__HEADER_END

#endif
