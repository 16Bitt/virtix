#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "clock.h"
#include "isr.h"

unsigned int pid;
virtix_proc_t* root;
virtix_proc_t* current_proc;

void scheduler(registers_t regs){
	cli();
	memcpy(current_proc->registers, &regs, sizeof(registers_t));
	
	current_proc = current_proc->next;
	while(current_proc->state == PROC_ASLEEP)
		current_proc = current_proc->next;

	memcpy(&regs, current_proc->registers, sizeof(registers_t));
	sti();
}

void init_procs(virtix_proc_t* process){
	root = process;

	process->next = process;
	process->last = process;
	process->thread = PROC_ROOT; //Unkillable
	process->state = PROC_RUNNING;

	start_timer(50);
	cli();
	register_interrupt_handler(32, scheduler);
	sti();
}

void kill_proc(unsigned int pid){
	vga_puts("WARN: kill_proc() is dummy stub\n");
}

unsigned int spawn_proc(virtix_proc_t* process){
	vga_puts("WARN: spawn_proc() is dummy stub\n");
	return pid++;
}

void susp_proc(unsigned int pid){
	virtix_proc_t* proc = pid_to_proc(pid);
	
	if(proc == PID_NOT_FOUND)
		return;

	proc->state = PROC_ASLEEP;
}

void wake_proc(unsigned int pid){
	virtix_proc_t* proc = pid_to_proc(pid);
	
	if(proc == PID_NOT_FOUND)
		return;

	proc->state = PROC_RUNNING;
}

unsigned int child_proc(virtix_proc_t* child, unsigned int pid){
	susp_proc(pid);

	virtix_proc_t* parent = pid_to_proc(pid);

	if(parent == PID_NOT_FOUND){
		PANIC("child process spawned in without parent");
	}

	child->parent = parent;
	
	return spawn_proc(child);
}

unsigned int fork_proc(unsigned int pid){
	vga_puts("WARN: fork_proc() is dummy stub\n");
	return pid++;
}

virtix_proc_t* pid_to_proc(unsigned int pid){
	virtix_proc_t* proc = root->next;

	while(proc != root){
		if(proc->pid == pid)
			return proc;

		proc = proc->next;
	}

	return PID_NOT_FOUND;
}
