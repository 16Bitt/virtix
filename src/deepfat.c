#include "common.h"
#include "vfs.h"
#include "fat.h"
#include "str-util.h"
#include "deepfat.h"

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
	//vga_puts("parse_dir(): making vfs node for '");
	//vga_puts(dir);
	//vga_puts("'\n");

	fs_node_t* node = mk_empty_dnode();
	//vga_puts("parse_dir(): reading .DIR file\n");
	char* fat_buff = (char*) fat_load_full(dir);
	
	//vga_puts(fat_buff);

	//vga_puts("parse_dir(): formatting .DIR in RAM\n");
	char* str = prep_str(fat_buff);

	//vga_puts("parse_dir(): verifying directory integrity\n");
	ASSERT(strcmp(str, "DEEPFAT") == 0);
	str = next_str(str);
	ASSERT(strcmp(str, "DEEPDIR") == 0);
	str = next_str(str);

	while(strcmp(str, "ENDDIR") != 0){
		//vga_puts("parse_dir(): reading deepFAT entry\n");
		char* ent = str;
		str = next_str(ent);
		
		//vga_puts("parse_dir(): found ent of type '");
		//vga_puts(ent);
		//vga_puts("'\n");

		fs_node_t* sub;

		if(strcmp(ent, "DIR") == 0){
			char* name = str;
			//vga_puts("parse_dir(): making dir for '");
			//vga_puts(name);
			//vga_puts("'\n");
			str = next_str(str);
			sub = parse_dir(fat_name_conv(str));
			
			strmov(sub->name, name);
			fat_dir_t* file = fat_dir_search(fat_name_conv(str));
			sub->inode	= file->cluster_low;
			sub->length	= file->bytes;
		}

		else if(strcmp(ent, "FIL") == 0){
			char* name = str;
			str = next_str(str);
			sub = mk_empty_fnode();
			
			//vga_puts("parse_dir(): searching for file '");
			//vga_puts(name);
			//vga_puts("'\n");
			
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
		//str = next_str(str);

		//vga_puts(str);
		
		node->link = sub;
		node = sub;
	}

	kfree(fat_buff);
}

void init_deepfat(){
	df_root = parse_dir("DFATROOTDIR");
}

uint df_read(fs_node_t* node, uint offset, uint size, char* buffer){
	return 0;
}

uint df_write(fs_node_t* node, uint offset, uint size, char* buffer){
	return 0;
}

void df_open(fs_node_t* node, uint index){
	return;
}

void df_close(fs_node_t* node){
	return;
}

struct dirent* df_readdir(fs_node_t* node, uint index){
	return NULL;
}

fs_node_t* df_finddir(fs_node_t* node, char* name){
	return NULL;
}
