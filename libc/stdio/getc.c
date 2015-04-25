#include "unistd.h"
#include "stdio.h"

int getc(FILE* stream){
	char c;
	int status = read(stream, &c, 1);
	
	if(status)
		return status;

	return c;
}
