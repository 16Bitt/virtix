#include "stdlib.h"
#include "syscall.h"

int close(int f){
	return syscall_1arg(SYS_CLOSE, f);
}
