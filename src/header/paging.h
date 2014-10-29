#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

#define PAGE_S 0x400000

extern unsigned int* current_dir;

typedef struct{
	unsigned int present	: 1;
	unsigned int rw		: 1;
	unsigned int user	: 1;
	unsigned int accessed	: 1;
	unsigned int dirty	: 1;
	unsigned int unused	: 7;
	unsigned int frame	: 20;
} page_t;

typedef struct{
	page_t pages[1024];
} page_table_t;

typedef struct{
	page_table_t* tables[1024];
	unsigned int tables_physical[1024];
	unsigned int physical_address;
} page_directory_t;

void init_paging();
void enable_paging();
void switch_page(unsigned int* page_dir);
void map_vpage_to_ppage(unsigned int vpage, unsigned int ppage);

//Create new pages
unsigned int* mk_page();
unsigned int* mk_page_dir();
void mmap_page(unsigned int* page, unsigned int vpage, unsigned int ppage);

void page_fault(registers_t regs);

#endif
