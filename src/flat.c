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

	proc->mmap.text		= kmalloc_a(PAGE_S); //0 M
	proc->mmap.bss		= kmalloc_a(PAGE_S); //4 M
	proc->mmap.data		= kmalloc_a(PAGE_S); //8 M
	proc->mmap.stack	= kmalloc_a(PAGE_S); //12 M

	return proc;
}
