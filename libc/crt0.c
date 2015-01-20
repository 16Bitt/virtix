#include "sys/cdefs.h"

extern int main(char** argc, int argv);

void _start(){
	main(NULL, NULL);
}
