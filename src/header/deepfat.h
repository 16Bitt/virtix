#ifndef DEEPFAT_H
#define DEEPFAT_H

#include "common.h"
#include "vfs.h"

//Root of the FS
extern fs_node_t* df_root;

void init_deepfat();

//VFS driver interfaces
uint df_read(fs_node_t* node, uint offset, uint size, uchar* buffer);
uint df_read(fs_node_t* node, uint offset, uint size, uchar* buffer);
void df_open(fs_node_t* node, uint index);
void df_close(fs_node_t* node);
struct dirent* df_readdir(fs_node_t* node, uint index);
fs_node_t* finddir_fs(fs_node_t* node, char* name);

//Convert DF inode to FAT filename
char* get_fat_name(uint inode);

#endif
