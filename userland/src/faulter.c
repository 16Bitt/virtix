#include "stdio.h"
#include "unistd.h"

int main(char** argv, int argc){
	printf("Going to segfault with pid %d\n", getpid());
	*((uint*) 0) = 123123;
	return 0;
}
