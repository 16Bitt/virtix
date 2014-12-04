#ifndef VFS_H
#define VFS_H

#include "common.h"

typedef uint (*read_type_t)(struct fs_node*, uint, uint, char*);
typedef uint (*write_type_t)(struct fs_node*, uint, uint, char*);
typedef uint (*open_type_t)(struct fs_node*);
typedef uint (*close_type_t)(struct fs_node*);

typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char* name);

typedef struct fs_node{
	char name[128];
	uint permissions;

	uint uid;
	uint gid;
	uint flags;

	uint inode;
	uint length;

	read_type_t	read;
	write_type_t	write;
	open_type_t	open;
	close_type_t	close;

	readdir_type_t	readdir;
	finddir_type_t	finddir;

	struct fs_node* link;
} vfs_node_t;

#endif
