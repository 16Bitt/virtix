#include "common.h"
#include "str-util.h"

char* prep_str(char* str){
	int i = 0;
	while(str[i] != 0)
		if((str[i] == ' ') || (str[i] == '\n'))
			str[i] = 0;

	return str;
}

char* next_str(char* str){
	str = &str[strlen(str)];
	
	int i = 0;
	while(str[i] == 0)
		i++;

	return &str[i];
}

void strmov(char* dest, char* src){
	int i;
	for(i = 0; i <= strlen(src); i++)
		dest[i] = src[i];
}
