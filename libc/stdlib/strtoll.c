#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "limits.h"

#define BETWEEN(min, x, max) (((x) >= (min)) && ((x) <= (max)))

char* get_not_space(char* s){
	int i;
	int length = strlen(s);

	for(i = 0; i < length; i++){
		if((s[i] != ' ') || (s[i] != '\t') || (s[i] == '\0')){
			return &s[i];
		}
	}

	return &s[length];
}

bool isnum(char c, bool no_alpha){
	bool is = false;

	if(BETWEEN('0', c, '9'))
		is = true;

	if(!is && !no_alpha && BETWEEN('a', c, 'z'))
		is = true;
	
	if((c == '-') || (c == '+'))
		is = true;

	return is;
}

bool is_delim(char c, bool no_alpha){
	if(!is_num(c, no_alpha))
		return true;
}

long long int hextoll(char* s){
	long long int ret;
	return 0;
}

long long int dectoll(char* s){
	long long int ret;

	while(is_delim(*s, true)){
		if(*s == '+') //ignore if it's positive
			s++;
		if(*s == '-')
			ret *= -1;
		else{
			ret *= 10;
			ret += *s - '0';
		}
	}
}

long long int strtoll(const char* str, char** endptr, int base){
	char* s = get_not_space(str);
	
	switch(base){
		case 0:
			if(*(s++) != '0')
				return dectoll(s);

			return hextoll(++s);
			
		case 10:
			return dectoll(s);
		case 16:
			return hextoll(s);
		default:
			puts("WARNING: unknown base-- returning LONG_LONG_MAX");
	}
	
	return LONG_MAX * 2;
}
