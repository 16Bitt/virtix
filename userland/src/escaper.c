#include "stdio.h"

int main(char** argv, int argc){
	printf("Testing an escape.\x1b[0;31m Done.\n");
	printf("Resetting. \x1b[0mDone.\n");
}
