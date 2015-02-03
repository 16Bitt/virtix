#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "fd.h"

fd_t fd_list[MAX_FD];

void init_fd(){
	memset(fd_list, 0, sizeof(fd_t) * MAX_FD);
}

fs_node_t* fd_lookup(uint fd){
	if(fd >= MAX_FD)
		return NULL;

	return fd_list[fd].node;
}

uint calc_blk_offset(uint bs, uint offs){
	uint index = 0;

	while(bs < offs){
		index++;
		offs--;
	}

	return index;
}

uint calc_buf_offset(uint bs, uint offs){
	return bs * calc_blk_offset(bs, offs) - offs;
}

uint fd_create(fs_node_t* node, uint offset){
	int i;
	for(i = 0; i < MAX_FD; i++){
		if(fd_list[i].present == 0){
			fd_list[i].node		= node;
			strmov(fd_list[i].name, node->name);
			fd_list[i].block_size	= node->dev->block_size;
			fd_list[i].buffer	= (char*) kmalloc(node->dev->block_size);
			fd_list[i].present	= 1;
			return i;
		}
	}

	return (uint) -1;
}

void fd_delete(uint fd){
	if(fd >= MAX_FD)
		return;
	
	kfree(fd_list[fd].buffer);
	memset(&fd_list[fd], 0, sizeof(fd_t));
}

void fd_seek(uint fd, uint offset){
	if(fd >= MAX_FD)
		return;

	fd_list[fd].offset = offset;
}

struct dirent* fd_readdir(uint fd){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;

	return readdir_fs(node, fd_list[fd].offset++);
}

uint fd_read(uint fd, uint size, char* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;

	
}

uint fd_write(uint fd, uint size, char* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;
	


}

uint fd_stat(uint fd, struct stat* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return (uint) -1;

	buffer->length = node->length;
	buffer->inode = node->inode;

	return 0;
}

uint fd_offset(uint fd){
	if(fd >= MAX_FD)
		return 0;

	return fd_list[fd].offset;
}
