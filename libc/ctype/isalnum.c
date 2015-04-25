#include "ctype.h"
#include "stdbool.h"

extern bool isnum(char, bool);

int isalnum(int c){
	return isnum((uchar) c, false);
}
