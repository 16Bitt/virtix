#include "common.h"
#include "dev.h"

uint stdout_write(fs_node_t* node, uint offset, uint size, char* buffer){
	int i;
	for(i = 0; i < size; i++)
		vga_putc(buffer[i]);

	return 0;
}

void dev_stdout(){
	mkdev("stdout", NULL, stdout_write);
}
