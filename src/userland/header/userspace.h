#ifndef USERSPACE_H
#define USERSPACE_H

#include "common.h"

/*
extern unsigned int c_err;
*/

typedef unsigned int uint;


#define SYS_FORK	0
#define SYS_GETPID	1
#define SYS_GETENV	2
#define SYS_WRITE	3
#define SYS_READ	4
#define SYS_CLOSE	5
#define SYS_EXIT	6

/*
uint fork();
*/

uint getpid();
char** get_env();

uint write(uint fid, char* buffer, size_t length);
uint read(uint fid, char* buffer, size_t length);
uint close(uint fid);

uint _exit(uint return_code);

#endif
