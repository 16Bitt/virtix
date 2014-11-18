#ifndef VIRTIX_PAGE_H
#define VIRTIX_PAGE_H

#include "common.h"
#include "paging.h"

typedef struct{
	page_table_t* tables[1024];
} vpage_dir_t;

extern vpage_dir_t* root_vpage_dir;

#define EMPTY_TAB ((page_table_t*) 0x00000002)

page_table_t* get_cr3();
unsigned int get_cr0();

void set_cr3(vpage_dir_t* dir);
inline void set_cr0(unsigned int new_cr0);

void switch_vpage_dir(vpage_dir_t* dir);

vpage_dir_t* mk_vpage_dir();
page_table_t* mk_vpage_table();

void virtix_page_init();

#endif
