#include "common.h"
#include "vfs.h"
#include "fat.h"
#include "str-util.h"

fs_node_t* df_root;

fs_node_t* mk_empty_node(){
	fs_node_t* node = (fs_node_t*) kmalloc(sizeof(fs_node_t));
	node->permissions = 777;
	node->uid = node->gid = node->flags = 0;
	node->link = NULL;

	return node;
}

fs_node_t* mk_empty_fnode(){
	fs_node_t* node = mk_empty_node();
	node->flags = FS_FILE;
	node->read = df_read;
	node->write = df_write;
	
	return node;
}

fs_node_t* parse_dir(char* dir){
	fs_node_t* node = mk_empty_dnode();`
	
	char* str = prep_str(dir);

	ASSERT(strcmp(str, "DEEPFAT") == 0);
	str = next_str(str);
	ASSERT(strcmp(str, "DEEPDIR") == 0);
	str = next_str(str);

	while(strcmp(str, "ENDDIR") == 0){
		char* ent = next_str(str);
		str = next_str(ent);
		
		fs_node_t* sub;

		if(strcmp(ent, "DIR") == 0){
			sub = mk_empty_dnode();

		}

		else if(strcmp(ent, "FIL") == 0){
			sub = mk_empty_fnode();
		}

		else	PANIC("bad deepFAT directory entry");
	}
}

void init_deepfat(){
	df_root = mk_empty_dnode();

