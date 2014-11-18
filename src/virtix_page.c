#include "common.h"
#include "monitor.h"
#include "virtix_page.h"
#include "kheap.h"

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

void set_cr0(unsigned int new_cr0){
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
