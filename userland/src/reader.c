#include "unistd.h"
#include "stdio.h"

int main(char** argv, int argc){
	puts("Going to read a line from a file");
	
	char buffer[128];
	FILE* f = open("/makefile", 0);
	fgets(buffer, 127, f);
	puts("fgets'd into buffer");
	printf("Read this line: %s\n", buffer);
	puts("Done.");
	return 0;
}
