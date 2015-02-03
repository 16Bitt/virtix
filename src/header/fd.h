#ifndef FD_H
#define FD_H

#include "common.h"
#include "vfs.h"

#define MAX_FD	16

typedef struct{
	char name[128];
	fs_node_t* node;
	
	uint offset;
	uint block;
	size_t block_size;
	
	char* buffer;

	uint mode;
	uint present;
} fd_t;

void init_fd();

fs_node_t* fd_lookup(uint fd);
uint fd_create(fs_node_t* node, uint offset);
void fd_delete(uint fd);

uint fd_offset(uint fd);

void fd_seek(uint fd, uint offset);
uint fd_read(uint fd, uint size, char* buffer);
uint fd_write(uint fd, uint size, char* buffer);
uint fd_flush(uint fd);
struct dirent* fd_readdir(uint fd);
uint fd_stat(uint fd, struct stat* buffer);

#endif
