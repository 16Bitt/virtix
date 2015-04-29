#include "unistd.h"
#include "syscall.h"
#include "errno.h"

int execve(char* path, char** argv, char** env){
	errno = syscall_3arg(SYS_EXECVE, (uint) path, (uint) argv, (uint) env);

	return errno;
}
