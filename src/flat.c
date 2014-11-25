#include "common.h"
#include "virtix_page.h"
#include "virtix_proc.h"

#define HANG()	cli(); \
		hlt();

virtix_proc_t* flat_load_bin(void* addr){
	cli();
	virtix_proc_t* proc = mk_empty_proc();
	proc->registers.useresp = 8;
	proc->registers.eip = 1024;
	
	proc->name = "FLAT_BIN";

	unsigned int mem = (unsigned int) kmalloc_a(PAGE_S);
	vpage_map(proc->cr3, 0, mem);
	memcpy((void*) mem, (void*) addr, 1024);
	
	mem = (unsigned int) kmalloc_a(PAGE_S);
	unsigned int esp = current_proc->registers.esp;
	unsigned int ss = current_proc->registers.ss;
	vpage_map(proc->cr3, (ss * 1024) + esp, mem);

	proc->registers.esp = esp;
	
	sti();
	return proc;
}
