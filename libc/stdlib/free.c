#include "stdlib.h"
#include "syscall.h"

void free(void* addr){
	(void) syscall_1arg(SYS_FREE, (uint) addr);
	return;
}
