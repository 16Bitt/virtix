#include "stdio.h"

int fputs(FILE* f, char* str){
	return fprintf(f, "%s\n", str);
}
