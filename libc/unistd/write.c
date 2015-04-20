#include "unistd.h"
#include "syscall.h"

int write(int fd, void* buffer, size_t length){
	return syscall_3arg(SYS_WRITE, (uint) fd, (uint) buffer, (uint) length);
}
