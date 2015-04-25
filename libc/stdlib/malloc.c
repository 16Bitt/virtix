#include "syscall.h"
#include "stdlib.h"

void* malloc(size_t s){
	return (void*) syscall_1arg(SYS_MALLOC, s);
}
