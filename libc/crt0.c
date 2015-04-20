#include "sys/cdefs.h"
#include "syscall.h"
#include "stdio.h"

extern int main(char** argc, int argv);
extern void _exit(int);

FILE* stdin	= NULL;
FILE* stdout	= NULL;
FILE* stderr	= NULL;

void _start(){
	stdin = syscall_0arg(SYS_GETSTDIN);
	stdout = syscall_0arg(SYS_GETSTDOUT);
	stderr = syscall_0arg(SYS_GETSTDERR);
	_exit(main(NULL, NULL));
}
