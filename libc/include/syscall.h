#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "sys/cdefs.h"

__HEADER_START

#define SYS_FORK	0
#define SYS_GETPID	1
#define SYS_GETENV	2
#define SYS_WRITE	3
#define SYS_READ	4
#define SYS_CLOSE	5
#define SYS_EXIT	6
#define SYS_OPEN	7
#define SYS_MALLOC	8
#define SYS_FREE	9
#define SYS_GET_ARGS	10
#define SYS_LSEEK	11
#define SYS_GETSTDIN	12
#define SYS_GETSTDOUT	13
#define SYS_GETSTDERR	14
#define SYS_EXEC	15
#define SYS_SBRK	16
#define SYS_RAISE	17
#define SYS_SIGNAL	18
#define SYS_SIGEXIT	19
#define SYS_WAIT	20
#define SYS_GETGID	21
#define SYS_GETCWD	22
#define SYS_CHDIR	23

int syscall_0arg(uint call_no);
int syscall_1arg(uint call_no, uint arg1);
int syscall_2arg(uint call_no, uint arg1, uint arg2);
int syscall_3arg(uint call_no, uint arg1, uint arg2, uint arg3);
int syscall_4arg(uint call_no, uint arg1, uint arg2, uint arg3, uint arg4);

__HEADER_END

#endif
