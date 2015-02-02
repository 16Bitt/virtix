#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "dev.h"
#include "deepfat.h"

void mkdev(char* name, read_block_t read, write_block_t write){
	if(fs_path(df_root, "/dev") == NULL){
		WARN("Can't make device, no path for /dev")
		return;
	}
	
	NOTIFY(name)

	char* path = (char*) kmalloc(strlen(name) + strlen("/dev/") + 2);
	strmov(path, "/dev/");
	strmov(&path[strlen("/dev/")], name);

	fs_node_t* node = vfs_touch(df_root, path);

	if(node == NULL){
		WARN("couldn't resolve path")
		return;
	}

	node->read_blk = read;
	node->write_blk = write;
}

uint dev_read(dev_t* dev, uint offset, char* buffer){
	if(dev->read == NULL){
		WARN("can't read from device");
		return (uint) -1;
	}

	return dev->read(dev, offset, buffer);
}


uint dev_write(dev_t* dev, uint offset, char* buffer){
	if(dev->write == NULL){
		WARN("can't write to device");
		return (uint) -1;
	}

	return dev->write(dev, offset, buffer);
}
