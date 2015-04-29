#include "ctype.h"

int isxdigit(int c){
	return isdigit(c) || C_BETWEEN('a', c, 'f') || C_BETWEEN('A', c, 'F');
}
