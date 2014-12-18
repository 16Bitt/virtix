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

uint open_fs(fs_node_t* node, uint offset){
	if(node->open != NULL)
		return node->open(node, offset);
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
	char* cpy = (char*) kmalloc(strlen(name) + 2);
	strmov(cpy, name);
	cpy[strlen(name) + 1] = 1;	//Any non-zero character works, solely for next_str
	
	name = cpy;
	char* end = &name[strlen(name) - 1];
	
	//Format for parsing (zero-style parsing)
	int i;
	for(i = 0; i < strlen(name); i++)
		if(name[i] == '/')
			name[i] = 0;
	
	//Iterate down the pathname
	do{
		node = node->link;

		while(node != NULL){
			if(strcmp(node->name, name) == 0)
				break;

			node = node->link;
		}

		if(node == NULL)
			return NULL;
		
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

void vfs_ls(fs_node_t* dir){
	fs_node_t* link = dir->link;

	while(link != NULL){
		vga_puts(link->name);
		vga_puts("\n");
		link = link->link;
	}
}

fs_node_t* vfs_get_dir(fs_node_t* node, char* name){
	//Copy the name for when we modify it
	char* cpy = (char*) kmalloc(strlen(name) + 2);
	strmov(cpy, name);
	cpy[strlen(name) + 1] = 1;	//Any non-zero character works, solely for next_str

	name = cpy;
	char* end = &name[strlen(name) - 1];
	
	//Set string end at last /
	int i;
	for(i = strlen(name); i > 0; i--)
		if(name[i] == '/'){
			name[i] = 0;
			break;
		}
	

	for(i = 0; i < strlen(name); i++)
		if(name[i] == '/')
			name[i] = 0;
	
	//Iterate down the pathname
	do{
		node = node->link;

		while(node != NULL){
			if(strcmp(node->name, name) == 0)
				break;

			node = node->link;
		}

		if(node == NULL)
			return NULL;
		
		name = next_str(name);
	}while((uint) name < (uint) end);
	
	//Clean up and yield
	kfree(name);
	return node;
}
