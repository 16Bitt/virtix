#include "signal.h"
#include "stdio.h"

int sigprocmask(int mask, const sigset_t* set, sigset_t* oset){
	printf("WARNING: sigprocmask is stubbed\n");
	return -1;
}
