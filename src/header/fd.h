#ifndef FD_H
#define FD_H

#include "common.h"
#include "vfs.h"

typedef struct{
	char* name;
	vfs_node_t* node;
	uint offset;
} fd_t;

fs_node_t* fd_lookup(int fd);

uint fd_write(int fd, uint offset, uint length, char* buffer);
uint fd_read(int fd, uint offset, uint length, char* buffer);
struct dirent* fd_readdir(int fd, uint index);

#endif
