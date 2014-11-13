#include "fs.h"
#include "common.h"
#include "kheap.h"
#include "monitor.h"
#include "kernel.h"

directory_t* fs_root = (directory_t*) 0;
unsigned int size = 0;

void mnt_initrd(unsigned int ptr){
	vga_puts_hex(ptr);
	unsigned int address = *((unsigned int*) ptr);
	size = *((unsigned int*) address);
	
	unsigned int magic_header = *(unsigned int*)(address + 4);
	unsigned int magic_footer = *(unsigned int*)(address + size - 4);

	ASSERT((magic_footer == FFS_MAGIC) && (magic_header == FFS_MAGIC));

	fs_root = (directory_t*) kmalloc(sizeof(directory_t));
	fs_root->start = address;

	unsigned int num_files = *(unsigned int*)(address + 8);
	fs_root->num_files = num_files;

	file_t** files_list = (file_t**) kmalloc(sizeof(file_t*) * num_files);
	
	int i;
	for(i = 0; i < num_files; i++){
		file_t* new_file = (file_t*) kmalloc(sizeof(file_t));
		files_list[i] = new_file;
		
		new_file->name 		= (char*) (address + 12 + (i * FFS_ENTRY_SIZE));

		vga_puts("CREATING: ");
		vga_puts(new_file->name);
		vga_putc('\t');
		

		new_file->offset 	= *((unsigned int*) (address + 12 + (i * FFS_ENTRY_SIZE) + 128));
		vga_puts("OFFSET: ");
		vga_puts_hex(new_file->offset);
		vga_putc('\t');


		new_file->size		= *((unsigned int*) (address + 12 + (i * FFS_ENTRY_SIZE) + 128 + 4));
		vga_puts("SIZE: ");
		vga_puts_hex(new_file->size);
		vga_putc('\n');

		new_file->data		= (unsigned char*) (address + new_file->offset);
	}

	fs_root->files = files_list;
}

int is_file(char* name){
	int i;
	for(i = 0; i < fs_root->num_files; i++)
		if(!strcmp(fs_root->files[i]->name, name))
			return 1;

	return 0;
}

unsigned int get_file_size(char* name){
	ASSERT(is_file(name));

	int i;
	for(i = 0; i < fs_root->num_files; i++)
		if(!strcmp(fs_root->files[i]->name, name))
			return fs_root->files[i]->size;
	
	return 0;
}

unsigned char* get_file_data(char* name){
	ASSERT(is_file(name));

	int i;
	for(i = 0; i < fs_root->num_files; i++)
		if(!strcmp(fs_root->files[i]->name, name))
			return fs_root->files[i]->data;
	
	return 0;
}
