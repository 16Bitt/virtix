#ifndef __STDLIB_H
#define __STDLIB_H

#include "sys/cdefs.h"

__HEADER_START

#include "sys/stat.h"
#include "sys/dirent.h"

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
void exit(int status);
void _exit(int status);

__HEADER_END

#endif
