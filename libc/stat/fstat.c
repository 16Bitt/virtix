#include "sys/stat.h"
#include "syscall.h"

int fstat(int fd, struct stat* st){
	return syscall_2arg(SYS_FSTAT, fd, (uint) st);
}
