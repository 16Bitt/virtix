#include "unistd.h"
#include "syscall.h"

int getgid(){
	return syscall_0arg(SYS_GETGID);
}
