#ifndef SYSCALL_H
#define SYSCALL_H

#include "common.h"
#include "isr.h"

#define SYS_GETPID	0
#define SYS_FORK	1
#define SYS_EXEC	2
#define SYS_OPEN	3
#define SYS_WRITE	4
#define SYS_READ	5

void syscall_handler(registers_t regs);

#endif
