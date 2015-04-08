#include "common.h"
#include "dev.h"
#include "vfs.h"

uint dev_null_read(struct fs_node* node, uint offset, uchar* buffer){
	int i;
	for(i = 0; i < 512; i++){
		buffer[i] = 0;
	}

	return 0;
}

void dev_null(){
	mkdev("null", dev_null_read, NULL);
}
