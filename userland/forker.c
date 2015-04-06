#include "stdlib.h"
#include "stdio.h"

int main(char** argc, int argv){
	printf("Starting program\n");
	printf("Fork returned %d", fork());
	printf("Program done\n");
	return;
}
