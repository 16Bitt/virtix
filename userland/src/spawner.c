#include "unistd.h"
#include "stdio.h"

int main(char** argv, int argc){
	exec("/bin/hello");
	printf("Successfully spawned subprocess!\n");
}
