#include "common.h"
#include "vfs.h"
#include "monitor.h"
#include "kheap.h"
#include "fs.h"
#include "stream.h"

#undef data

//heap_t* vheap;
vfs_node_t* vfs_root = NULL;

vfs_node_t* get_file(char* name){
	if(vfs_root == NULL)
		return NULL;

	vfs_node_t* current = vfs_root;
	while(current != NULL){
		if(!strcmp(current->name, name))
			return current;

		current = current->next;
	}

	return NULL;
}

void init_vfs(){
	//vheap = kbmalloc("VFS");
	
	//Copy every part of the ramdisk into the new (actual) ramdisk
	int i;
	for(i = 0; i < fs_root->num_files; i++)
			vfs_ramload(fs_root->files[i]->name, fs_root->files[i]->data, fs_root->files[i]->size);
}

void vfs_ramload(char* name, void* address, unsigned int size){
	//Create new buffers to copy everything
	vfs_node_t* new_file = (vfs_node_t*) kmalloc(sizeof(vfs_node_t));
	char* new_name = (char*) kmalloc(strlen(name) + 1);
	void* new_data = kmalloc(size);
	
	//Start copying
	memcpy(new_name, name, strlen(name) + 1);
	memcpy(new_data, address, size);
	
	//If it already exists in the table, delete it
	if(vfs_isfile(name) == FILE_EXIST)
		vfs_rm(name);

	new_file->name = new_name;
	new_file->buffer = new_data;
	new_file->size = size;

	//Make new root
	if(vfs_root == NULL){
		vfs_root = new_file;
		new_file->last = NULL;
		new_file->next = NULL;
	}

	//Add to tree
	else{
		new_file->next = vfs_root->next;
		vfs_root->next = new_file;
		new_file->last = vfs_root;
	}
}

unsigned int vfs_assoc(char* name, stream_t* stream){
	vfs_ramload(name, stream->buffer, stream->end_pos);
	return 0;
}

unsigned int vfs_rm(char* name){
	if(vfs_isfile(name) == FILE_NO_EXIST)
		return 0xFFFFFFFF;

	vfs_node_t* deleting = get_file(name);
	if(deleting->last == NULL){
		vfs_root = deleting->next;
		vfs_root->last = NULL;
	}

	else{
		deleting->last->next = deleting->next;
		deleting->next->last = deleting->last;
	}

	kfree(deleting->name);
	kfree(deleting->buffer);
	kfree(deleting);

	return 0;
}

unsigned int vfs_cp(char* orig, char* new_file){
	if(vfs_isfile(orig) == FILE_NO_EXIST)
		return 0xFFFFFFFF;

	vfs_node_t* copying = get_file(orig);
	vfs_ramload(new_file, copying->buffer, copying->size);

	return 0;
}

unsigned int vfs_reassoc(char* name, char* new_name){
	if(vfs_isfile(name) == FILE_NO_EXIST)
		return 0xFFFFFFFF;

	vfs_node_t* renaming = get_file(name);
	
	//If the file exists, toss it
	if(vfs_isfile(new_name) == FILE_EXIST)
		vfs_rm(new_name);

	//Manually ensure no memory leaks/invalid references
	char* new_name_buf = (char*) kmalloc(strlen(new_name) + 1);
	kfree(renaming->name);
	memcpy(new_name_buf, new_name, strlen(new_name) + 1);
	renaming->name = new_name_buf;

	return 0;
}

unsigned int vfs_isfile(char* name){
	if(get_file(name) == NULL)
		return FILE_NO_EXIST;

	return FILE_EXIST;
}

unsigned int vfs_open(char* name, unsigned int mode){
	if(vfs_isfile(name) == FILE_NO_EXIST)
		return 0xFFFFFFFF;

	vfs_node_t* file = get_file(name);
	unsigned int fid = mk_stream();
	char* buffer = (char*) file->buffer;

	int i;
	for(i = 0; i < file->size; i++)
		fwrite_char(fid, buffer[i]);

	if(mode == FILE_BOT)
		stream_rewind(fid);

	return fid;
}

void vfs_list(unsigned int stream_id){
	vfs_node_t* current = vfs_root;

	while(current != NULL){
		int i;
		for(i = 0; i < strlen(current->name); i++)
			fwrite_char(stream_id, current->name[i]);
		
		fwrite_char(stream_id, '\n');

		current = current->next;
	}
}
