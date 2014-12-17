#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "fd.h"

fd_t* fd_list;

void init_fd(){
	fd_list = (fd_t*) kmalloc(sizeof(fd_t) * MAX_FD);
	memset(fd_list, 0, sizeof(fd_t) * MAX_FD);
}

fs_node_t* fd_lookup(uint fd){
	if(fd >= MAX_FD)
		return NULL;

	return fd_list[fd].node;
}

uint fd_create(fs_node_t* node, uint offset){
	int i;
	for(i = 0; i < MAX_FD; i++){
		vga_puts("fd_create(): none\n");
		if(fd_list[i].present == 0){
			fd_list[i].offset	= offset;
			fd_list[i].node		= node;
			strmov(fd_list[i].name, node->name);
			fd_list[i].present	= 1;
			return i;
		}
	}

	return (uint) -1;
}

void fd_delete(uint fd){
	if(fd >= MAX_FD)
		return;

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

	uint hold = fd_list[fd].offset;
	fd_list[fd].offset += size;
	return read_fs(node, hold, size, buffer);
}

uint fd_write(uint fd, uint size, char* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;
	
	uint hold = fd_list[fd].offset;
	fd_list[fd].offset += size;
	return write_fs(node, hold, size, buffer);
}
