#include "common.h"
#include "dev.h"
#include "vfs.h"
#include "deepfat.h"
#include "monitor.h"

uint stdout_write(fs_node_t* node, uint offset, uchar* buffer){
	vga_putc(*buffer);

	return 0;
}

void dev_stdout(){
	mkdev("stdout", NULL, stdout_write);
	fs_node_t* node = fs_path(df_root, "/dev/stdout");
	node->dev->block_size = 0;
}
