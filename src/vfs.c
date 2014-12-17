#include "common.h"
#include "monitor.h"
#include "kheap.h"
#include "vfs.h"
#include "str-util.h"

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

void open_fs(fs_node_t* node, uint offset){
	if(node->open != NULL)
		node->open(node, offset);
}

void close_fs(fs_node_t* node){
	if(node->close != NULL)
		node->close(node);
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

fs_node_t* fs_path(fs_node_t* node, char* name){
	//Copy the name for when we modify it
	char* cpy = (char*) kmalloc(strlen(name) + 1);
	strmov(cpy, name);
	name = cpy;
	char* end = &name[strlen(name)];
	
	//Format for parsing (zero-style parsing)
	int i;
	for(i = 0; i < strlen(name); i++)
		if(name[i] == '/')
			name[i] = 0;

	//Iterate down the pathname
	do{
		node = finddir_fs(node, name);
		if(node == NULL){
			kfree(cpy);
			return NULL;
		}

		name = next_str(name);
	}while((uint) name < (uint) end);
	
	//Clean up and yield
	kfree(name);
	return node;
}

struct dirent* readdir_generic(fs_node_t* node, uint index){
	fs_node_t* link = node->link;

	while(index != 0){
		if(link == NULL)
			return NULL;

		link = link->link;
		index--;
	}

	if(link == NULL)
		return NULL;

	struct dirent* dir = (struct dirent*) kmalloc(sizeof(struct dirent));
	strmov(dir->name, link->name);
	dir->ino = link->inode;
	return dir;
}
