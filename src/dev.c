#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "dev.h"
#include "deepfat.h"

void mkdev(char* name, read_type_t read, write_type_t write){
	if(fs_path(df_root, "/dev") == NULL){
		WARN("Can't make device, no path for /dev")
		return;
	}
	
	char* path = (char*) kmalloc(strlen(name) + strlen("/dev/") + 2);
	strmov(path, "/dev/");
	strmov(&path[strlen("/dev/")], name);

	fs_node_t* node = vfs_touch(df_root, path);

	if(node == NULL){
		WARN("couldn't resolve path")
		return;
	}

	node->read = read;
	node->write = write;
}
