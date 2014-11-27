#ifndef VIRTIX_PAGE_H
#define VIRTIX_PAGE_H

#include "common.h"
#include "paging.h"

typedef struct{
	page_table_t* tables[1024];
} vpage_dir_t;

extern vpage_dir_t* root_vpage_dir;

#define EMPTY_TAB ((page_table_t*) 0x00000002)

page_table_t* get_cr3();			//contents of cr3 register
unsigned int get_cr0();				//contents of cr0 register

void set_cr3(vpage_dir_t* dir);			//set cr3 register
inline void set_cr0(unsigned int new_cr0);	//set cr0 register

void switch_vpage_dir(vpage_dir_t* dir);	//Change address space

vpage_dir_t* mk_vpage_dir();			//Make empty root page dir
page_table_t* mk_vpage_table();

void virtix_page_init();			//Start paging

void vpage_map(vpage_dir_t* cr3, unsigned int virt, unsigned int phys);
void vpage_map_user(vpage_dir_t* cr3, unsigned int virt, unsigned int phys);

void convert_vpage(vpage_dir_t* kdir);		//Change page to usermode

void dump_page(vpage_dir_t* dir, unsigned int address);

#endif
