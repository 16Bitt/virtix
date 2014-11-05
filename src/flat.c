#include "common.h"
#include "paging.h"
#include "virtix_proc.h"

#define HANG()	cli(); \
		hlt();

virtix_proc_t* flat_load_bin(void* addr){
	virtix_proc_t* proc = mk_empty_proc();
	proc->registers.esp = 1023;
	proc->registers.eip = 1024;
	
	proc->name = "FLAT_BIN";

	mmap_page(proc->cr3, 0, kmalloc_a(PAGE_S));

	return proc;
}
