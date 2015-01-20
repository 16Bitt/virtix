#include "stdlib.h"
#include "syscall.h"

int getpid(){
	return syscall_0arg(SYS_GETPID);
}
