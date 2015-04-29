#include "ctype.h"

int ispunct(int c){
	return (isprint(c) && !isalpha(c) && !isdigit(c));
}
