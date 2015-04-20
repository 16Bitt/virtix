#include "unistd.h"
#include "syscall.h"

int read(int fd, void* buffer, size_t length){
	return syscall_3arg(SYS_READ, (uint) fd, (uint) buffer, (uint) length);
}
