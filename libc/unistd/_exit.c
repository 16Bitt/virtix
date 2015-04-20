#include "unistd.h"
#include "syscall.h"

void _exit(int status){
	(void) syscall_1arg(SYS_EXIT, (uint) status);
	for(;;);
}
