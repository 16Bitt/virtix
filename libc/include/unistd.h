#ifndef UNISTD_H
#define UNISTD_H

#include "sys/cdefs.h"

__HEADER_START

#include "sys/stat.h"
#include "sys/dirent.h"
#include "stdio.h"

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

//File system manipulation
void sync();
int close(int fd);
int open(char* name, uint position);
int write(int fd, void* addr, size_t length);
int read(int fd, void* addr, size_t length);
int lseek(int fd, int addr, int dir);

//File execution
int exec(char* path);
void _exit(int retcode);
void exit(int status);int pipe(int fd[2]);
int stat(char* path, struct stat* st);
int fstat(int file, struct stat* st);
int getgid();
int getpid();
int fork();

//Directories
int chdir(char* path);
char* getcwd(char* path, size_t size);

__HEADER_END

#endif
