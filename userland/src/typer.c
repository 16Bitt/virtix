#include "stdio.h"
#include "unistd.h"

int main(char** argv, int argc){
	FILE* f = fopen("/typed.txt", "w");

	print("Mash some keys! > ");

	for(;;)
		fprintf("%c", fgetc(stdin), f);

	return 0;
}
