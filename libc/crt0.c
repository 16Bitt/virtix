#include "sys/cdefs.h"

extern int main(char** argc, int argv);
extern void _exit(int);

void _start(){
	//asm volatile ("int $30");
	//main(NULL, NULL);
	//_exit(0);
}
