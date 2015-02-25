#include "stdlib.h"
#include "stdio.h"
void break_program();

int main(char** argc, int argv){
	printf("Hello, world! I am PID %d.\n", getpid());
	return 0;
}

void break_program(){
	*((unsigned int*) 0xFFFFFFFF) = 1;
}
