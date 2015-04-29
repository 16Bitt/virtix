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

#define _PATH_BSHELL	"/bin/sh"
#define _PATH_TTY	"/dev/tty"
#define _PATH_DEVNULL	"/dev/null"

#define W_OK	1
#define R_OK	2
#define X_OK	4

#define PATH_MAX	2048

int access(const char* path, int flags);

//File system manipulation
void sync();
int close(int fd);
int write(int fd, void* addr, size_t length);
int read(int fd, void* addr, size_t length);
int lseek(int fd, int addr, int dir);

//File execution
int exec(char* path);
int execve(char* path, char** argv, char** env);
void _exit(int retcode);
void exit(int status);int pipe(int fd[2]);
int getgid();
int getpid();
int fork();

//Directories
int chdir(char* path);
char* getcwd(char* path, size_t size);

__HEADER_END

#endif
