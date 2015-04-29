#ifndef USERSPACE_H
#define USERSPACE_H

#include "common.h"
#include "virtix_proc.h"

extern unsigned int c_err;
typedef unsigned int uint;

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
#define SYS_EXECVE	24

#define	SYS_STAT	25
#define SYS_FSTAT	26

uint fork();
uint getpid();
char** get_env();

uint write(uint fid, char* buffer, size_t length);
uint read(uint fid, char* buffer, size_t length);
uint close(uint fid);
uint open(char* path, uint offset);
int sys_wait(int pid, int* status, int options);

uint _exit(registers_t* regs);

void init_userspace();	//Maps userspace call handler

#endif
