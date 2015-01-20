#include "stdlib.h"
#include "syscall.h"

int open(char* path, uint offset){
	return syscall_2arg(SYS_OPEN, (uint) path, offset);
}
