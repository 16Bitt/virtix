#ifndef __STDLIB_H
#define __STDLIB_H

#include "sys/cdefs.h"

__HEADER_START

//Used to query the kernel
int virtix_syscall(uint call_no);
int virtix_syscall0(uint call_no, uint val0);
int virtix_syscall1(uint call_no, uint val0, uint val1);
int virtix_syscall2(uint call_no, uint val0, uint val2, uint val3);

//Basic file IO
int open(char* name, uint offset);
int close(int file);
int read(int file, void* buffer, size_t length);
int write(int file, void* buffer, size_t length);
int readdir(int file, struct dirent* entry);

//File manipulation
int lseek(int file, int direction, int mode);
int stat(char* path, struct stat* st);
int fstat(int file, struct stat* st);
int unlink(char* path);

//Process manipulation
int fork();
int exit(int err_code);

__HEADER_END

#endif
