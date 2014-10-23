#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "clock.h"
#include "isr.h"

unsigned int pid;
virtix_proc_t* root;
virtix_proc_t* current;

void scheduler(registers_t regs){
	cli();
	memcpy(current->registers, &regs, sizeof(registers_t));
	
	current = current->next;
	while(current->state == PROC_ASLEEP)
		current = current->next;

	memcpy(&regs, current->registers, sizeof(registers_t));
	sti();
}

void init_procs(virtix_proc_t* process){
	root = process;

	process->next = process;
	process->last = process;
	process->thread = PROC_ROOT; //Unkillable
	process->state = PROC_PROC;

	start_timer(50);
	register_interrupt_handler(32, scheduler);
}
