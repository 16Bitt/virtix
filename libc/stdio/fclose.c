#include "stdio.h"
#include "unistd.h"

int fclose(FILE* f){
	return close((int) f);
}
