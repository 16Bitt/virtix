#include "stdio.h"
#include "unistd.h"

int main(char** argv, int argc){
	FILE* f = (FILE*) open("/test.txt", 0);
	
	print("Mash some keys! > ");
	
	char c;

	for(;;){
		fputc(f, c = fgetc(stdin));
		if(c == 'x')
			break;
	}
	
	close(f);
	puts("\nDone.");
	sync();

	return 0;
}
