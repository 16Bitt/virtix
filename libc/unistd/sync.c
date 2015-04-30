#include "unistd.h"
#include "syscall.h"

void sync(){
	(void) syscall_0arg(SYS_SYNC);
}
