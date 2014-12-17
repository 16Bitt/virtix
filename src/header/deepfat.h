#ifndef DEEPFAT_H
#define DEEPFAT_H

#include "common.h"
#include "vfs.h"

//Root of the FS
extern struct fs_node* df_root;

void init_deepfat();

//VFS driver interfaces
uint df_read	(struct fs_node* node, uint offset, uint size, char* buffer);
uint df_write	(struct fs_node* node, uint offset, uint size, char* buffer);
uint df_open	(struct fs_node* node, uint index);
void df_close	(struct fs_node* node);

struct dirent* df_readdir(struct fs_node* node, uint index);
struct fs_node* df_finddir(struct fs_node* node, char* name);

//Convert DF inode to FAT filename
char* get_fat_name(uint inode);

//Utilities for rapid file deployment
struct fs_node* mk_empty_node();
struct fs_node* mk_empty_fnode();
struct fs_node* mk_empty_dnode();

#endif
