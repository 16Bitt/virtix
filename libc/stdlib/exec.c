#include "stdlib.h"
#include "syscall.h"

int exec(char* file){
	return syscall_1arg(SYS_EXEC, (uint) file);
}
