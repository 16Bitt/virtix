#include "common.h"

//Basic memory movement routines
void* memset(void* s, int c, size_t n){
	char* str = (char*) s;
	
	int i;
	for(i = 0; i < n; i++)
		str[i] = (char) c;

	return s;
}

void* memcpy(void* dest, const void* src, size_t n){
	char* dest_str = (char*) dest;
	char* src_str = (char*) src;

	int i;
	for(i = 0; i < n; i++)
		dest_str[i] = src_str[i];

	return dest;
}

size_t strlen(char* str){
	size_t len = 0;
	while(str[len])
		len++;
	
	return len;
}

int strcmp(char* str1, char* str2){
	if(strlen(str1) != strlen(str2))
		return -1;

	size_t end = strlen(str1);
	int i;
	for(i = 0; i < end; i++)
		if(str1[i] != str2[i])
			return -1;

	return 0;
}
