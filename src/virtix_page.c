#include "common.h"
#include "monitor.h"
#include "virtix_page.h"
#include "kheap.h"
#include "virtix_proc.h"

vpage_dir_t* current_vpage_dir = NULL;
vpage_dir_t* root_vpage_dir = NULL;

//Assembly abstraction for more maintainable code
page_table_t* get_cr3(){
	unsigned int cr3;

	asm volatile ("movl %%cr3, %%eax" : "=a" (cr3));
	return (page_table_t*) cr3;
}

unsigned int get_cr0(){
	unsigned int cr0;

	asm volatile ("movl %%cr0, %%eax" : "=a" (cr0));
	return cr0;
}

void set_cr3(vpage_dir_t* dir){
	unsigned int addr = (unsigned int) &dir->tables[0];
	asm volatile ("movl %%eax, %%cr3" :: "a" (addr));
}

inline void set_cr0(unsigned int new_cr0){
	asm volatile ("movl %%eax, %%cr0" :: "a" (new_cr0));
}

void switch_vpage_dir(vpage_dir_t* dir){
	set_cr3(dir);
	set_cr0(get_cr0() | 0x80000000);
}

//Utility code for paging
vpage_dir_t* mk_vpage_dir(){
	vpage_dir_t* dir = (vpage_dir_t*) kmalloc_a(sizeof(vpage_dir_t));
	
	int i;
	for(i = 0; i < 1024; i++)
		dir->tables[i] = EMPTY_TAB;

	return dir;
}

page_table_t* mk_vpage_table(){
	page_table_t* tab = (page_table_t*) kmalloc_a(sizeof(page_table_t));

	int i;
	for(i = 0; i < 1024; i++){
		tab->pages[i].present	= 0;
		tab->pages[i].rw	= 1;
	}

	return tab;
}

void vpage_map(vpage_dir_t* dir, unsigned int phys, unsigned int virt){
	short id = virt >> 22;
	page_table_t* tab = mk_vpage_table();
	
	dir->tables[id] = ((page_table_t*)((unsigned int) tab | 3)); //Writeable/present
	
	int i;
	for(i = 0; i < 1024; i++){
		tab->pages[i].frame = phys >> 12;
		tab->pages[i].present = 1;
		phys += 4096;
	}
}

void vpage_map_user(vpage_dir_t* dir, unsigned int phys, unsigned int virt){
	short id = virt >> 22;
	page_table_t* tab = mk_vpage_table();
	
	dir->tables[id] = ((page_table_t*)((unsigned int) tab | 3 | 4)); //Writeable/present + usermode
	
	int i;
	for(i = 0; i < 1024; i++){
		tab->pages[i].frame = phys >> 12;
		tab->pages[i].present = 1;
		tab->pages[i].user = 1;
		phys += 4096;
	}
}

void vpage_fault(registers_t* regs){
	cli();
	if(current_proc != NULL){
		memcpy(&current_proc->registers, regs, sizeof(registers_t));
		dump_proc(current_proc);
	}
	
	vga_set_fg(RED);

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
	if(re)		vga_puts(" (RE) ");
	

	PANIC("vpage fault");
}

void virtix_page_init(){
	current_vpage_dir = mk_vpage_dir();
	root_vpage_dir = current_vpage_dir;

	unsigned int i;
	for(i = 0; i < 0xF0000000; i += PAGE_S)
		vpage_map(root_vpage_dir, i, i);
	
	register_interrupt_handler(14, vpage_fault);
	cli();
	switch_vpage_dir(root_vpage_dir);
}

void convert_vpage(vpage_dir_t* kdir){
	int i;
	for(i = 0; i < 1024; i++){
		kdir->tables[i] = (page_table_t*) ((unsigned int) kdir->tables[i] | 4); //Set usermode bit
		
		if(((unsigned int) kdir->tables[i]) & 1){ 				//The page is present
			int j;
			for(j = 0; j < 1024; j++)
				kdir->tables[i]->pages[j].user = 1;			//Make every page within run in usermode
		}
	}
}

void dump_page(vpage_dir_t* dir, unsigned int address){
	unsigned short id = address >> 22;
	DISP("Index salt =", (unsigned int) dir->tables[id]);
}

vpage_dir_t* copy_user_dir(vpage_dir_t* dir){
	uint i;

	vpage_dir_t* copy = mk_vpage_dir();
	memcpy(copy, root_vpage_dir, sizeof(vpage_dir_t));

	for(i = 0; i < 1024; i++){
		if(((uint) dir->tables[i]) & 4){
			vga_fmt("Found a user page at index %d\n", i);
			page_table_t* tab = (page_table_t*) ((uint) dir->tables[i] & 0xFFFFF000);
			vga_fmt("Table at address %X maps to %X\n", (uint) tab, i << 22);
			vga_fmt("Virtually mapped from %X\n", tab->pages[0].frame << 12);

			void* buffer = kmalloc_a(PAGE_S);
			memcpy(buffer, (void*) (tab->pages[0].frame << 12), PAGE_S);
			vpage_map_user(copy, (uint) buffer, (uint) i << 22);
		}
	}

	return copy;
}
