#include "common.h"
#include "paging.h"
#include "virtix_proc.h"

virtix_proc_t* flat_load_bin(void* addr){
	virtix_proc_t* proc = mk_empty_proc();
	proc->registers->esp = 1023;
	proc->registers->eip = 1024;

	unsigned int* dir = mk_page_dir();
	void* page = kmalloc_a(PAGE_S);
	mmap_page(dir, 0, page);
	memcpy(page + 1024, addr, PAGE_S / 4);
	proc->cr3 = current_dir;

	return proc;
}
