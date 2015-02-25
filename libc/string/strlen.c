#include "string.h"

size_t strlen(char* s){
	int len = 0;
	while(*s != '\0'){
		len++;
		s++;
	}

	return len;
}
