#include "common.h"
#include "virtix_page.h"
#include "virtix_proc.h"

#define HANG()	cli(); \
		hlt();

virtix_proc_t* flat_load_bin(void* addr){
	cli();
	virtix_proc_t* proc = mk_empty_proc();
	proc->registers.useresp = 0;
	proc->registers.eip = 0xF0000000;
	
	proc->name = "FLAT_BIN";

	unsigned int mem = (unsigned int) kmalloc_a(PAGE_S);
	vpage_map_user(proc->cr3, mem, 0xF0000000);
	memcpy((void*) mem, (void*) addr, 1024);
	
	sti();
	return proc;
}
