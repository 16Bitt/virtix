#include "sys/stat.h"
#include "syscall.h"

int stat(const char* path, struct stat* st){
	return syscall_2arg(SYS_STAT, (uint) path, (uint) st);
}
