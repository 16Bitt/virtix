#include "stdlib.h"
#include "stdio.h"

int fgetc(FILE* f){
	char c;
	int status = read(f, &c, 1);

	if(status != 0)
		return status;

	return c;
}
