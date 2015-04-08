#include "stdio.h"
#include "stdlib.h"

int main(char** argv, int argc){
	printf("Going to segfault with pid %d", getpid());
	*((uint*) 0) = 123123;
	return 0;
}
