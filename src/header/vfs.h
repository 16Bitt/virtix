#ifndef VFS_H
#define VFS_H

//The aim of this header is to provide basic
//POSIX VFS style operation for the OS

#include "common.h"
#include "dev.h"

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

	dev_t* dev;

	open_type_t	open;
	close_type_t	close;
	
	read_block_t	read_blk;
	write_block_t	write_blk;

	readdir_type_t	readdir;
	finddir_type_t	finddir;
	
	struct fs_node* holds;
	struct fs_node* link;
} fs_node_t;

struct dirent{
	char name[128];
	uint ino;
};

struct stat{
	uint st_dev;
	uint st_ino;
	uint st_mode;
	uint st_nlink;
	uint st_uid;
	uint st_gid;
	uint st_rdev;
	uint st_size;
	uint st_atime;
	uint st_mtime;
	uint st_ctime;
	uint st_blksize;
	uint st_blocks;
};

#define FS_FILE		1
#define FS_DIRECTORY	2
#define FS_CHARDEVICE	3
#define FS_BLOCKDEVICE	4
#define FS_PIPE		5
#define FS_SYMLINK	6
#define FS_MOUNTPOINT	8

extern fs_node_t* root_node;

uint read_fs(fs_node_t* node, uint offset, uchar* buffer);
uint write_fs(fs_node_t* node, uint offset, uchar* buffer);

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

void vfs_ls(char* path);

#endif
