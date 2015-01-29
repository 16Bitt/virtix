#include "dev.h"
#include "monitor.h"

extern uchar vga_color;

uint dev_stderr_write(fs_node_t* node, uint offset, uint size, char* buffer){
	uchar hold = vga_color;
	vga_set_fg(RED);
	
	int i;
	for(i = 0; i < size; i++)
		vga_putc(buffer[i]);

	vga_color = hold;
	return 0;
}

void dev_stderr(){
	mkdev("stderr", NULL, dev_stderr_write);
}