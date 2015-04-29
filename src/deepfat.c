#include "common.h"
#include "vfs.h"
#include "fat.h"
#include "str-util.h"
#include "deepfat.h"
#include "fd.h"
#include "monitor.h"
#include "dev.h"
#include "kheap.h"

fs_node_t* df_root;
uint current_inode = 0;
dev_t*  df_dev;

fs_node_t* mk_empty_node(){
	fs_node_t* node = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	memset(node, 0, sizeof(fs_node_t));
	
	node->permissions = 777;
	node->uid = node->gid = node->flags = 0;
	
	node->open	= df_open;
	node->close	= df_close;
	
	node->dev	= df_dev;
	node->link	= NULL;

	return node;
}

fs_node_t* mk_empty_fnode(){
	fs_node_t* node = mk_empty_node();
	
	node->flags	= FS_FILE;
	node->read_blk	= df_read_blk;
	node->write_blk	= df_write_blk;

	return node;
}

fs_node_t* mk_empty_dnode(){
	fs_node_t* node = mk_empty_node();
	
	node->flags = FS_DIRECTORY;
	node->readdir = df_readdir;
	node->finddir = df_finddir;
	
	return node;
}

fs_node_t* parse_dir(char* dir){
	fs_node_t* node = mk_empty_dnode();
	fs_node_t* parent = node;
	char* fat_buff = (char*) fat_load_full(dir);

	char* str = prep_str(fat_buff);

	ASSERT(strcmp(str, "DEEPFAT") == 0);
	str = next_str(str);
	ASSERT(strcmp(str, "DEEPDIR") == 0);
	str = next_str(str);

	while(strcmp(str, "ENDDIR") != 0){
		char* ent = str;
		str = next_str(ent);
		fs_node_t* sub;
		
		/*
		vga_puts("\t* creating '");
		vga_puts(str);
		vga_puts("'\n");
		*/

		if(strcmp(ent, "DIR") == 0){
			char* name = str;
			str = next_str(str);
			sub = parse_dir(fat_name_conv(str));
			
			strmov(sub->name, name);
			strmov(sub->dos_name, str);
			fat_dir_t* file = fat_dir_search(fat_name_conv(str));
			sub->inode	= file->cluster_low;
			sub->length	= file->bytes;
			sub->flags	= FS_DIRECTORY;

			//Callbacks for basic VFS manipulation
			sub->open	= df_open;
			sub->close	= df_close;
			sub->readdir	= readdir_generic;
			sub->finddir	= df_finddir;
		}

		else if(strcmp(ent, "FIL") == 0){
			char* name = str;
			str = next_str(str);
			sub = mk_empty_fnode();
			
			strmov(sub->dos_name, str);
			fat_dir_t* file = fat_dir_search(fat_name_conv(str));
			
			strmov(sub->name, name);
			sub->inode	= file->cluster_low;
			sub->length	= file->bytes;
		}

		else{
			PANIC("bad deepFAT directory entry");
		}

		str = ent;
		str = next_str(str);
		str = next_str(str);
		str = next_str(str);
	
		if(parent->holds == NULL){
			parent->holds = sub;
			node = sub;
		}

		else{
			node->link = sub;
			node = sub;
		}
	}
	
	kfree(fat_buff);
	return parent;
}

void init_deepfat(){
	df_dev	= (dev_t*) kmalloc(sizeof(dev_t));
	df_dev->read = NULL;
	df_dev->write = NULL;
	df_dev->block_size = CLUSTER_BSIZE;
	df_root = parse_dir(fat_name_conv("DFATROOT.DIR"));
}

uint df_read_blk(fs_node_t* node, uint offset, uchar* buffer){
	return fat_read_block(node->dos_name, offset, buffer);
}

uint df_write_blk(fs_node_t* node, uint offset, uchar* buffer){
	uint status = fat_write_block(node->dos_name, offset, buffer);
	fat_dir_t* dir = fat_search(node->dos_name);
	if(dir != NULL)
		dir->bytes = node->length;
	return status;
}

uint df_open(fs_node_t* node, uint index){
	return fd_create(node, index);
}

void df_close(fs_node_t* node){
	vga_puts("WARN: df_close() is dummy stub\n");
	return;
}

struct dirent* df_readdir(fs_node_t* node, uint index){
	return NULL;
}

fs_node_t* df_finddir(fs_node_t* node, char* name){
	return NULL;
}

char fat_generated[13];
char* df_mk_name(){
	char* lookup = "0123456789ABCDEF";
	char* extension = "FIL";
	
	fat_generated[8] = 0;
	
	int i, index;
	for(i = 28, index = 0; i > -1; i -= 4, index++){
		fat_generated[index] = lookup[(current_inode >> i) & 0xF];
	}

	current_inode++;
	
	fat_generated[8] = '.';

	for(i = 0; i < 3; i++)
		fat_generated[9 + i] = extension[i];
	
	fat_generated[12] = 0;

	return fat_generated;
}

fat_dir_t* df_new_file(){
	return fat_create(df_mk_name());
}

void df_sync(){
	
}
