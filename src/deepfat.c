#include "common.h"
#include "vfs.h"
#include "fat.h"
#include "str-util.h"
#include "deepfat.h"
#include "fd.h"

fs_node_t* df_root;

fs_node_t* mk_empty_node(){
	fs_node_t* node = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	memset(node, 0, sizeof(fs_node_t));
	
	node->permissions = 777;
	node->uid = node->gid = node->flags = 0;
	
	node->open	= df_open;
	node->close	= df_close;

	return node;
}

fs_node_t* mk_empty_fnode(){
	fs_node_t* node = mk_empty_node();
	
	node->flags	= FS_FILE;
	node->read	= df_read;
	node->write	= df_write;

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
			sub->flags	= FS_FILE;

			//Callbacks for basic file manipulation
			sub->write	= df_write;
			sub->read	= df_read;
			sub->open	= df_open;
			sub->close	= df_close;
		}

		else{
			PANIC("bad deepFAT directory entry");
		}

		str = ent;
		str = next_str(str);
		str = next_str(str);
		str = next_str(str);
		//str = next_str(str);

		//vga_puts(str);
		
		node->link = sub;
		node = sub;
	}
	
	kfree(fat_buff);
	return parent;
}

void init_deepfat(){
	df_root = parse_dir("DFATROOTDIR");
}

uint df_read(fs_node_t* node, uint offset, uint size, char* buffer){
	return fat_read(node->dos_name, offset, size, buffer);
}

uint df_write(fs_node_t* node, uint offset, uint size, char* buffer){
	return fat_write(node->dos_name, offset, size, buffer);
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
