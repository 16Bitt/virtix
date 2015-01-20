#include "stdlib.h"
#include "syscall.h"

int fork(){
	return syscall_0arg(SYS_FORK);
}
