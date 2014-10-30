#include "common.h"
#include "paging.h"
#include "virtix_proc.h"

#define HANG()	cli(); \
		hlt();

virtix_proc_t* flat_load_bin(void* addr){
	virtix_proc_t* proc = mk_empty_proc();
	proc->registers->esp = 1023;
	proc->registers->eip = 1024;

	unsigned int* dir = mk_page_dir();	//Page directory
	void* page = kmalloc_a(PAGE_S);		//Buffer

	mmap_page(dir, 0x1000, page);		//&directory[0] = page
	proc->cr3 = dir;
	
	vga_puts_hex(dir);
	vga_puts_hex(dir[0]);
	HANG()

	return proc;
}
