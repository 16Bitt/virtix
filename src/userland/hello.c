#include "userspace.h"

char* str = "Hello, world! From userspace!\n";

void main(){
	write(0, str, strlen(str));
	_exit(0);
}
