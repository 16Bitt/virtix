#include "stdio.h"
#include "stdlib.h"

int main(char** argv, int argc){
	void* x = malloc(10);
	printf("Malloc'd 10 bytes at %X\n", (uint) x);
	free(x);
	printf("Freed\n");

	return 0;
}
