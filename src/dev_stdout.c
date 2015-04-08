#include "common.h"
#include "dev.h"
#include "vfs.h"
#include "monitor.h"

uint stdout_write(fs_node_t* node, uint offset, uchar* buffer){
	int i;
	for(i = 0; i < node->dev->block_size; i++)
		vga_putc(buffer[i]);

	return 0;
}

void dev_stdout(){
	mkdev("stdout", NULL, stdout_write);
}
