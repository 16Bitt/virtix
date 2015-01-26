#include "stdlib.h"
#include "syscall.h"

int lseek(int file, int direction, int mode){
	return syscall_3arg(SYS_LSEEK, file, direction, mode);
}
