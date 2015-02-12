#include "sys/cdefs.h"

extern int main(char** argc, int argv);
extern void _exit(int);

void _start(){
	main(NULL, NULL);
	_exit(0);
}
