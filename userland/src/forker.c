#include "unistd.h"
#include "stdio.h"
#include "sys/wait.h"

int main(char** argc, int argv){
	int f;
	printf("Starting program with pid %d\n", getpid());
	printf("Fork returned %d\n", f = fork());
	
	int x;
	
	
	if(f == 0)
		wait(&x);
	else
		exec("/bin/hello");
	

	printf("pid %d done.\n", getpid());

	if(f == 0)
		printf("Child returned %d.\n", x);

	return -1;
}
