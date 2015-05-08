#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "sys/stat.h"

char inbuffer[256];

void collapse_input(char* str);
void prompt(char* str);
void split(char* str);
char* iter(char* str, char* end);
void exec_builtin();
void badsh_error(char* msg);
bool file_exists(char* path);

int main(char** argv, int argc){
	while(true){
		prompt("badsh> ");
		exec_builtin();
	}

	return 0;
}


//Remove the backspaces from a string
void collapse_input(char* str){
	int len = strlen(str);
	
	int i, j;
	for(i = 0; i < len; i++)
		if(str[i] == '\b')
			for(j = i; j < len; j++)
				str[j] = str[j + 1];
	
}

//Update inbuffer with a given text prompt
void prompt(char* str){
	printf("%s", str);
	char* ret = fgets(inbuffer, 255, stdin);
	if(ret == NULL){
		puts("Bye!");
		exit(0);
	}
	collapse_input(inbuffer);
}

//Replace all spaces, tabs and newlines with delimiting 0's
void split(char* str){
	int len = strlen(str);
	
	int i;
	for(i = 0; i < len; i++)
		if((str[i] == '\t') || (str[i] == ' ') || (str[i] == '\n'))
			str[i] = 0;
}

//Iterate over a split string until end is reached
char* iter(char* str, char* end){
	str += strlen(str);
	while(str < end){
		str++;
		if(*str != 0)
			return str;
	}

	return NULL;
}

//Split the line up and try to execute it
void exec_builtin(){
	char* end = &inbuffer[strlen(inbuffer)];
	split(inbuffer);
	char* str = inbuffer;
	
	if(strlen(str) == 0)
		puts("");
	
	else if(strcmp("echo", str) == 0){
		str = iter(str, end);
		while(str != NULL){
			printf("%s ", str);
			str = iter(str, end);
		}
		
		putchar('\n');
	}

	else if(strcmp("exec", str) == 0){
		str = iter(str, end);
		if(str == NULL)
			badsh_error("exec needs an argument");
		
		if(file_exists(str) == 0){
			badsh_error("file not found");
			return;
		}

		int status;
		if(fork() == 0)
			exec(str);

		else{
			wait(&status);
			printf("program returned %d exit status.\n", status);
		}
	}
	
	else
		badsh_error("unknown command");

	//Clear the buffer
	int i;
	for(i = 0; i < 256; i++)
		inbuffer[i] = 0;
}

//Display an error msg
void badsh_error(char* msg){
	printf("! Badsh error: %s\n", msg);
}

bool file_exists(char* path){
	struct stat buffer;
	return stat(path, &buffer) == 0;
}
