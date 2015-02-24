#include "common.h"
#include "dev.h"
#include "vfs.h"
#include "monitor.h"
#include "deepfat.h"
#include "isr.h"

extern uchar last_key;

uint dev_stdin_read(fs_node_t* node, uint offset, char* buffer){
	wait_for_update:
		*buffer = last_key;
	
	if(last_key == 0){
		hlt();
		goto wait_for_update;
	}
	
	return 0;
}

void dev_stdin(){
	mkdev("stdin", dev_stdin_read, NULL);
	fs_node_t* node = fs_path(df_root, "/dev/stdin");
	node->dev->block_size = 1;
	last_key = 0;
	node->length = 0xFFFFFFFF;
}
