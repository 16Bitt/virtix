#include "stdio.h"
#include "unistd.h"

char* fgets(char* s, int size, FILE* file){
	int i;
	for(i = 0; i < size; i++){
		int status = fgetc(file);
		if(status < 0)
			return NULL;

		s[i] = (unsigned char) status;
		
		if((unsigned char) status == '\n')
			return s;
	}

	return s;
}
