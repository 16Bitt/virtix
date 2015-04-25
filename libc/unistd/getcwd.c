#include "sys/param.h"
#include "unistd.h"
#include "syscall.h"

char* getcwd(char* buf, size_t size){
	return (char*) syscall_1arg(SYS_GETCWD, buf);
}
