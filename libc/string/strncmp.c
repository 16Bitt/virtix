#include "string.h"

int strncmp(char* str1, char* str2, size_t length){
	int i;
	for(i = 0; i < length; i++)
		if(str1[i] != str2[i])
			return i + 1;

	return 0;
}
