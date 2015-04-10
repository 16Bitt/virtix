#include "string.h"

int strcmp(char* str1, char* str2){
	size_t len = strlen(str1);
	if(len != strlen(str2))
		return 1;

	int i;
	for(i = 0; i < len; i++)
		if(str1[i] != str2[i])
			return i + 1;
	
	return 0;
}
