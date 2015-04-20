#include "sys/wait.h"
#include "syscall.h"

pid_t waitpid(pid_t pid, int* status, int options){
	int ret;

	while((ret = syscall_3arg(SYS_WAIT, pid, (uint) status, options)) == 0);
		//printf("Got %d from SYS_WAIT\n", ret);

	return ret;
}
