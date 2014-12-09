#include "common.h"
#include "vfs.h"
#include "fat.h"
#include "str-util.h"

fs_node_t* df_root;

fs_node_t* mk_empty_node(){
	fs_node_t* node = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	memset(node, 0, sizeof(fs_node_t));
	
	node->permissions = 777;
	node->uid = node->gid = node->flags = 0;

	return node;
}

fs_node_t* mk_empty_fnode(){
	fs_node_t* node = mk_empty_node();
	
	node->flags = FS_FILE;
	node->read = df_read;
	node->write = df_write;

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
	void* fat_buff = fat_load_full(dir);
	
	char* str = prep_str(fat_buff);

	ASSERT(strcmp(str, "DEEPFAT") == 0);
	str = next_str(str);
	ASSERT(strcmp(str, "DEEPDIR") == 0);
	str = next_str(str);

	while(strcmp(str, "ENDDIR") == 0){
		char* ent = next_str(str);
		str = next_str(ent);
		
		fs_node_t* sub;

		if(strcmp(ent, "DIR") == 0){
			sub = parse_dir(str);
			fat_dir_t* file = fat_dir_search(str);
			sub->inode	= file->cluster_low;
			sub->length	= file->bytes;
		}

		else if(strcmp(ent, "FIL") == 0){
			sub = mk_empty_fnode();
			fat_dir_t* file = fat_dir_search(str);
			sub->inode	= file->cluster_low;
			sub->length	= file->bytes;
		}

		else PANIC("bad deepFAT directory entry");
	
		str = next_str(str);
		int i;
		for(i = 0; i < strlen(str); i++)
			sub->name[i] = str[i];
		sub->name[strlen(str)] = 0;
	
		node->link = sub;
		node = sub;
	}

	kfree(fat_buff);
}

void init_deepfat(){
	df_root = parse_dir("DFATROOTDIR");
}


