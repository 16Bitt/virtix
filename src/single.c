#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "isr.h"

virtix_proc_t* proc_bottom = NULL;

//These values are read by hardload function
unsigned int hl_cr3;
unsigned int hl_eip;
unsigned int hl_esp;

unsigned int sp_child(){
	return (unsigned int) -1;
}

void enter_userspace(virtix_proc_t* proc){
	proc_bottom = proc;
	proc->next = NULL;
	hl_eip = proc->registers.eip;
	hl_esp = proc->registers.esp;
	switch_vpage_dir(proc->cr3);
	
	current_proc = proc;

	sti();
	hard_usermode();
}

void single_yield(virtix_proc_t* proc, registers_t* regs){
	memcpy(&proc_bottom->registers, regs, sizeof(registers_t));
	
	if(proc == proc_bottom)
		PANIC("Can't return from parent process");

	proc->next = proc_bottom;
	proc_bottom = proc;
	
	memcpy(regs, &proc->registers, sizeof(registers_t));
	switch_vpage_dir(proc->cr3);
}

unsigned int single_exit(registers_t* regs){
	unsigned int hold = regs->ebx;
	proc_bottom = proc_bottom->next;

	if(proc_bottom == NULL)
		PANIC("return from process with no parent");

	memcpy(regs, &proc_bottom->registers, sizeof(registers_t));
	switch_vpage_dir(proc_bottom->cr3);

	return hold;
}
