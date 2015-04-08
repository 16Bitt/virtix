#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "dev.h"
#include "deepfat.h"
#include "kheap.h"
#include "str-util.h"

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
	
	node->dev = (dev_t*) kmalloc(sizeof(dev_t));
	node->dev->read = read;
	node->dev->write = write;
	node->dev->block_size = 512;
}

uint dev_read(fs_node_t* dev, uint offset, char* buffer){
	if(dev->read_blk == NULL){
		WARN("can't read from device");
		return (uint) -1;
	}

	return dev->read_blk(dev, offset, buffer);
}


uint dev_write(fs_node_t* dev, uint offset, char* buffer){
	if(dev->write_blk == NULL){
		WARN("can't write to device");
		return (uint) -1;
	}

	return dev->write_blk(dev, offset, buffer);
}
