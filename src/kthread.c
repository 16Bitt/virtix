#include "common.h"
#include "virtix_proc.h"
#include "monitor.h"
#include "isr.h"

void mk_kthread(char* name, void* addr){
	cli();
	virtix_proc_t* proc = mk_empty_proc();
	memcpy(&proc->registers, &current_proc->registers, sizeof(registers_t));
	sti();
	proc->name = name;
	proc->registers.eip = (unsigned int) addr;
	proc->registers.useresp = ((unsigned int) kmalloc_a(512)) - 500;
	spawn_proc(proc);
}

void kthread_hlt(){
	vga_puts("kthread halted\n");	
	int i;
	for(;;);
}

void kthread_exit(){
	unsigned int pid = current_proc->pid;
	kill_proc(pid);
	kthread_hlt();
}