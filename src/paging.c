#include "common.h"
#include "monitor.h"
#include "paging.h"
#include "isr.h"
#include "virtix_proc.h"

unsigned int* current_dir		= 0;
unsigned int* process_dir		= 0;

unsigned int* root_dir		= 0;

static unsigned int page_dir_location	= 0;
static unsigned int* last_page		= 0;

static unsigned int proc_dir		= 0;

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
	//page_dir_location = (unsigned int) &current_dir[0];
	asm volatile ("mov %%eax, %%cr3" :: "a" (page_dir_location));
	asm volatile ("mov %cr0, %eax");
	asm volatile ("orl $0x80000000, %eax");
	asm volatile ("mov %eax, %cr0");
}

void switch_page(unsigned int* page_dir){
	current_dir = page_dir;
	asm volatile("mov %0, %%cr3":: "r"(page_dir));
	u32int cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void init_paging(){
	current_dir		= (unsigned int*) PAGE_S;
	page_dir_location	= (unsigned int) current_dir;
	last_page		= (unsigned int*) (PAGE_S + 4096);
	root_dir		= current_dir;

	unsigned int i;
	for(i = 0; i < 1024; i++)
		current_dir[i] = 0 | 2;

	for(i = 0; i < 0x8000000; i += PAGE_S)
		map_vpage_to_ppage(i, i);	//Identity mapping the entire kernel
	
	register_interrupt_handler(14, page_fault);
	cli();
	
	switch_page(root_dir);
}

void page_fault(registers_t* regs){
	cli();
	if(current_proc != NULL){
		dump_proc(current_proc);
	}

	unsigned int err_pos;
	asm volatile ("mov %%cr2, %0" : "=r" (err_pos));

	vga_puts("Page fault occurred at ");
	vga_puts_hex(err_pos);
	
	vga_puts("\nReasons:");

	int no_page = regs->err_code & 1;
	int rw = regs->err_code & 2;
	int um = regs->err_code & 4;
	int re = regs->err_code & 8;
	int dc = regs->err_code & 16;

	if(dc)		vga_puts(" (Instruction decode error) ");
	if(!no_page)	vga_puts(" (No page present) ");
	if(um)		vga_puts(" (in user mode) ");
	if(rw)		vga_puts(" (Write permissions) ");
	if(re)			vga_puts(" (RE) ");
	

	PANIC("PAGE FAULT");
}

void mmap_page(unsigned int* page_dir, unsigned int vpage, unsigned int ppage){
	short id = vpage >> 22;
	
	unsigned int* page = mk_page();				//CONFIRMED

	int i;
	for(i = 0; i < 1024; i++){
		page[i] = ppage  | 2 | 1;			//User mode, RW, present
		ppage += 4096;
	}

	page_dir[id] = ((unsigned int) page)  | 2 | 1;	//User mode, RW, present
}

unsigned int* mk_page(){					//WORKS
	unsigned int* page = (unsigned int*) kmalloc_a(4096);
	memset(page, 0, 4096);
	return page;
}

unsigned int* mk_page_dir(){				//WORKS
	unsigned int* dir = (unsigned int*) kmalloc_a(4096);
	
	int i;
	for(i = 0; i < 1024; i++)
		dir[i] =  2 | 0; //User mode, RW, not present
	
	return (unsigned int*) dir;
}

void dump_page(unsigned int* dir){
	vga_puts("Dumping page at ");
	vga_puts_hex(dir);
	vga_puts(":\n");

	int i;
	for(i = 0; i < 4; i++){
		vga_puts_hex(dir[i]);
		vga_puts(" ");
	}

	vga_puts("\n");
}