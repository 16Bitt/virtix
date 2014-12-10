#ifndef VFS_H
#define VFS_H

//The aim of this header is to provide basic
//POSIX VFS style operation for the OS

#include "common.h"

struct fs_node;

typedef uint (*read_type_t)	(struct fs_node*, uint, uint, char*);
typedef uint (*write_type_t)	(struct fs_node*, uint, uint, char*);
typedef void (*open_type_t)	(struct fs_node*, uint);
typedef void (*close_type_t)	(struct fs_node*);

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
} fs_node_t;

struct dirent{
	char name[128];
	uint ino;
};

#define FS_FILE		1
#define FS_DIRECTORY	2
#define FS_CHARDEVICE	3
#define FS_BLOCKDEVICE	4
#define FS_PIPE		5
#define FS_SYMLINK	6
#define FS_MOUNTPOINT	8

extern fs_node_t* root_node;

uint read_fs(fs_node_t* node, uint offset, uint size, uchar* buffer);
uint write_fs(fs_node_t* node, uint offset, uint size, uchar* buffer);

void open_fs(fs_node_t* node);
void close_fs(fs_node_t* node);

struct dirent* readdir_fs(fs_node_t* node, uint index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);

#endif
