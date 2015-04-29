#include "stdio.h"
#include "unistd.h"

FILE* fopen(const char* path, const char* mode){
	int flags = 0;
	while(*mode != '\0'){
		switch(*mode){
			case 'r':
			case 'w':
			case '+':
				flags |= O_RDWR;
				break;
			case 'a':
				flags |= O_APPEND;
			default:
				return -1;
		}

		mode++;
	}

	return (FILE*) open(path, flags);
}
