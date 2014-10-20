#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

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
void map_vpage_to_ppage(unsigned int vpage, unsigned int ppage);

void page_fault(registers_t regs);

#endif
