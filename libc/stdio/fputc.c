#include "stdio.h"
#include "unistd.h"

int fputc(FILE* file, char c){
	return fputchar(file, c);
}
