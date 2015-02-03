#include "common.h"
#include "monitor.h"
#include "kheap.h"
#include "vfs.h"
#include "str-util.h"
#include "deepfat.h"

uint read_fs(fs_node_t* node, uint offset, uchar* buffer){
	if(node->read_blk != NULL)
		return node->read_blk(node->dev, offset, buffer);
	else
		return 0;
}

uint write_fs(fs_node_t* node, uint offset, uchar* buffer){
	if(node->write_blk != NULL)
		return node->write_blk(node->dev, offset, buffer);
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
	if(strcmp(name, "/") == 0)
		return node;

	char* cpy = (char*) kmalloc(strlen(name) + 2);
	strmov(cpy, name);
	cpy[strlen(name) + 1] = 1;	//Any non-zero character works, solely for next_str
	
	name = cpy;
	char* end = &name[strlen(name)];
	
	//Format for parsing (zero-style parsing)
	int i, len = strlen(name);
	for(i = 0; i < len; i++)
		if(name[i] == '/')
			name[i] = 0;

	if(strlen(name) == 0)
		name = next_str(name);

	//Iterate down the pathname
	do{
		node = node->holds;

		while(node != NULL){
			if(strcmp(node->name, name) == 0)
				break;

			node = node->link;
		}

		if(node == NULL){
			return NULL;
		}
		
		name = next_str(name);
	}while((uint) name < (uint) end);

	kfree(cpy);
	return node;
}

struct dirent* readdir_generic(fs_node_t* node, uint index){
	fs_node_t* link = node->holds;

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

void vfs_ls(char* path){
	fs_node_t* dir = fs_path(df_root, path);
	if(dir == NULL){
		vga_puts("vfs_ls(): invalid path\n");
		return;
	}

	fs_node_t* link = dir->holds;
	
	vga_puts("directory listing for ");
	vga_puts(path);
	vga_puts("\n");
	while(link != NULL){
		vga_puts(link->name);
		if(link->flags == FS_DIRECTORY)
			vga_puts("/");
		vga_puts("\n");
		link = link->link;
	}
}

fs_node_t* vfs_get_dir(fs_node_t* node, char* name){
	char* cpy = (char*) kmalloc(strlen(name) + 2);
	char* ref = cpy;
	strmov(cpy, name);
	cpy[strlen(name) + 1] = 1;	//Any nonzero value works here
	name = cpy;

	int i;
	for(i = strlen(name); i >= 0; i--)
		if(name[i] == '/'){
			name[i] = 0;
			break;
		}
	
	fs_node_t* output;
	if(strlen(name) == 0)
		output = df_root;
	else
		output = fs_path(node, name);
	
	kfree(ref);
	return output;
}

char* basename(char* name){
	int i;
	for(i = strlen(name); i >= 0; i--)
		if(name[i] == '/')
			return &name[i + 1];

	return name;
}

fs_node_t* vfs_touch(fs_node_t* node, char* name){
	fs_node_t* dir = vfs_get_dir(node, name);

	if(dir == NULL)
		return NULL;
	
	fs_node_t* file = mk_empty_fnode();
	strmov(file->name, basename(name));

	fat_dir_t* entry = df_new_file();

	if(entry == NULL)
		PANIC("kernel attempted to overwrite file in FAT");

	int i;
	for(i = 0; i < 11; i++){
		file->dos_name[i] = entry->name[i];
	}
	
	file->link = dir->holds;
	dir->holds = file;

	return file;
}

fs_node_t* vfs_mkdir(fs_node_t* node, char* name){
	fs_node_t* dir = vfs_get_dir(node, name);
	if(dir == NULL){
		vga_puts("vfs_mkdir(): invalid path\n");
		return NULL;
	}
	
	fs_node_t* file = mk_empty_dnode();
	strmov(file->name, basename(name));

	file->link = dir->holds;
	dir->holds = file;

	return file;
}
