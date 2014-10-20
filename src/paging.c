#include "common.h"
#include "monitor.h"
#include "paging.h"
#include "isr.h"

static unsigned int* current_dir	= 0;
static unsigned int page_dir_location	= 0;
static unsigned int* last_page		= 0;

extern unsigned int end;

void map_vpage_to_ppage(unsigned int virtual, unsigned int physical){
	short id = virtual >> 22;

	int i;
	for(i = 0; i < 1024; i++){
		last_page[i] = physical | 3;
		physical += 4096;
	}

	current_dir[id] = ((unsigned int) last_page) | 3;
	last_page = (unsigned int*) (((unsigned int) last_page) + 4096);
}

void enable_paging(){
	asm volatile ("mov %%eax, %%cr3" :: "a" (page_dir_location));
	asm volatile ("mov %cr0, %eax");
	asm volatile ("orl $0x80000000, %eax");
	asm volatile ("mov %eax, %cr0");
}

void init_paging(){
	current_dir		= (unsigned int*) 0x400000;
	page_dir_location	= (unsigned int) current_dir;
	last_page		= (unsigned int*) 0x404000;

	unsigned int i;
	for(i = 0; i < 1024; i++)
		current_dir[i] = 0 | 2;

	for(i = 0; i < 0x8000000; i += 0x400000)
		map_vpage_to_ppage(i, i);	//Identity mapping the entire kernel
	
	register_interrupt_handler(14, page_fault);
	cli();
	
	enable_paging();
}

void page_fault(registers_t registers){
	PANIC("PAGE FAULT");
}
