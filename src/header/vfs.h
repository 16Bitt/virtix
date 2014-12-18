#ifndef VFS_H
#define VFS_H

//The aim of this header is to provide basic
//POSIX VFS style operation for the OS

#include "common.h"

struct fs_node;

typedef uint (*read_type_t)	(struct fs_node*, uint, uint, char*);
typedef uint (*write_type_t)	(struct fs_node*, uint, uint, char*);
typedef uint (*open_type_t)	(struct fs_node*, uint);
typedef void (*close_type_t)	(struct fs_node*);

typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char* name);

typedef struct fs_node{
	char name[128];
	uint permissions;

	uint uid;
	uint gid;
	uint flags;
		
	char dos_name[16];
	uint inode;
	uint length;

	read_type_t	read;
	write_type_t	write;
	open_type_t	open;
	close_type_t	close;

	readdir_type_t	readdir;
	finddir_type_t	finddir;
	
	struct fs_node* holds;
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

uint open_fs(fs_node_t* node, uint offset);
void close_fs(fs_node_t* node);

struct dirent* readdir_fs(fs_node_t* node, uint index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);
fs_node_t* fs_path(fs_node_t* node, char* name);
fs_node_t* vfs_get_dir(fs_node_t* node, char* name);
char* basename(char* path);
struct dirent* readdir_generic(fs_node_t* node, uint index);

fs_node_t* vfs_touch(fs_node_t* node, char* path);
fs_node_t* vfs_mkdir(fs_node_t* node, char* path);

void vfs_ls(fs_node_t* dir);

#endif
