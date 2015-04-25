#include "unistd.h"
#include "stdio.h"

//For the love of god, don't ever use this.
char* gets(char* s){
	return fgets(s, 0xFFFFFFFF, stdin);
}
