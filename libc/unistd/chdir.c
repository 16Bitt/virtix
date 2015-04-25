#include "unistd.h"
#include "syscall.h"

int chdir(char* path){
	return syscall_1arg(SYS_CHDIR, (uint) path);
}
