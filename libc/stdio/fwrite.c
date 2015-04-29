#include "stdio.h"
#include "unistd.h"

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* fd){
	int i;
	for(i = 0; i < nmemb; i++){
		write((int) fd,  ptr, size);
		ptr += size;
	}

	return nmemb;
}
