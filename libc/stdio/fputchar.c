#include "stdio.h"

int fputchar(FILE* f, char c){
	return write(f, &c, 1);
}
