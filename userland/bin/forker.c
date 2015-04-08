#include "stdlib.h"
#include "stdio.h"

int main(char** argc, int argv){
	int f;
	printf("Starting program\n");
	printf("Fork returned %d\n", f = fork());

	if(f != 0)
		exec("/bin/faulter");

	printf("Program done\n");
	return;
}
