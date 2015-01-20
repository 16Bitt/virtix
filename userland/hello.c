#include "stdlib.h"

extern int write(int, void*, size_t);
extern void _exit(int);

int main(char** argc, int argv){
	write(1, "Hello, World!\n", 14);
	_exit(0);
}
