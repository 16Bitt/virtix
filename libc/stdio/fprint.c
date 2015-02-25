#include "stdio.h"
#include "string.h"

int fprint(FILE* f, char* s){
	return write(f, s, strlen(s));
}
