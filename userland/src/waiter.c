#include "stdio.h"
#include "sys/wait.h"
#include "unistd.h"

int main(char** argv, int argc){
	int stat;
	
	if(fork()){
		exec("/bin/hello");
		return 0;
	}

	waitpid(-1, &stat, 0);
	return 0;
}
