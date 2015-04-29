#include "ctype.h"

int iscntrl(int c){
	return !isgraph(c) && !isspace(c);
}
