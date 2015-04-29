#include "unistd.h"
#include "stdio.h"

char inbuffer[256];

int main(char** argv, int argc){
	fgets(inbuffer, 256, stdin);

	return 0;
}
