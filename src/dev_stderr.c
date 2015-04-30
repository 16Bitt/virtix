#include "dev.h"
#include "monitor.h"
#include "vfs.h"
#include "deepfat.h"
#include "tty.h"

extern uchar vga_color;

uint dev_stderr_write(fs_node_t* node, uint offset, uchar* buffer){
	tty_putc(*buffer);
	return 0;
}

void dev_stderr(){
	mkdev("stderr", NULL, dev_stderr_write);
	fs_node_t* node = fs_path(df_root, "/dev/stderr");
	node->dev->block_size = 0;
}
