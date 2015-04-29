#include "string.h"

char* strstr(char* haystack, char* needle){
	size_t length = strlen(needle);
	size_t haylen = strlen(haystack);

	int i;
	for(i = 0; i < haylen - length; i++){
		if(strncmp(&haystack[i], needle, length) == 0)
			return &haystack[i];
	}

	return NULL;
}
