#include "ctype.h"

int isprint(int c){
	return (isprint(c) || isdigit(c) || isalpha(c) || isblank(c) || isspace(c));
}
