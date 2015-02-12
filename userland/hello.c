#include "stdlib.h"
extern int close(int f);

void break_program();

int main(char** argc, int argv){
	int f = open("/dev/stdout", 0);
	write(f, "Hello, World!\n", 14);
	close(f);
	return 0;
}

void break_program(){
	*((unsigned int*) 0xFFFFFFFF) = 1;
}
