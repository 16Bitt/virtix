#include "common.h"
#include "monitor.h"
#include "kheap.h"
#include "vfs.h"

uint read_fs(fs_node_t* node, uint offset, uint size, uchar* buffer){
	if(node->read != NULL)
		return node->read(node, offset, size, buffer);
	else
		return 0;
}

uint write_fs(fs_node_t* node, uint offset, uint size, uchar* buffer){
	if(node->write != NULL)
		return node->write(node, offset, size, buffer);
	else
		return 0;
}

void open_fs(fs_node_t* node){
	if(node->open != NULL)
		node->open(node);
}

void close_fs(fs_node_t* node){
	if(node->close != NULL)
		node->open(node);
}

struct dirent* readdir_fs(fs_node_t* node, uint index){
	if((node->readdir != NULL) && ((node->flags & 7) == FS_DIRECTORY))
		return node->readdir(node, index);
	else
		return (struct dirent*) NULL;
}

fs_node_t* finddir_fs(fs_node_t* node, char* name){
	if((node->readdir != NULL) && ((node->flags & 7) == FS_DIRECTORY))
		return node->finddir(node, name);
	else
		return (fs_node_t*) NULL;
}
