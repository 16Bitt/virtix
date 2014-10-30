#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "clock.h"
#include "isr.h"
#include "paging.h"

unsigned int pid = 0;
virtix_proc_t* root;
virtix_proc_t* current_proc = NULL;

registers_t hold_root;

int x = 0;

#define HANG if(x){cli();hlt();}

void scheduler(registers_t regs){
	//vga_puts("scheduler(): copying data\n");
	memcpy(current_proc->registers, &regs, sizeof(registers_t));
	

	//vga_puts("scheduler(): finding open process\n");
	current_proc = current_proc->next;
	while(current_proc->state == PROC_ASLEEP){
		vga_puts("scheduler(): skipping sleeping thread\n");
		current_proc = current_proc->next;
	}
	//vga_puts("scheduler(): found open process\n");
	memcpy(&regs, current_proc->registers, sizeof(registers_t));
	
	//vga_puts("scheduler(): copied registers\n");
	//switch_page(current_proc->cr3);
}

void force_stub(registers_t regs){
	sti();
	vga_puts("force_stub(): initializing processes\n");
	memcpy(&hold_root, &regs, sizeof(registers_t));
	vga_puts("force_stub(): exiting\n");
}

void init_procs(void* goto_here){
	register_interrupt_handler(31, force_stub);
	asm volatile ("int $31");
	
	vga_puts("init_procs(): captured root\n");

	root = mk_empty_proc();
	root->registers = &hold_root;
	root->pid	= pid++;
	root->cr3	= current_dir;
	root->next	= root;
	root->last	= root;
	root->thread	= PROC_ROOT; //Unkillable
	root->state	= PROC_RUNNING;
	
	current_proc = root;

	current_proc->registers->eip = (unsigned int) goto_here;

	cli();
	start_timer(5);
	cli();
	register_interrupt_handler(32, scheduler);
	sti();
}

void kill_proc(unsigned int pid){
	vga_puts("WARN: kill_proc() is dummy stub\n");
}

unsigned int spawn_proc(virtix_proc_t* process){
	process->pid = pid++;
	
	virtix_proc_t* hold = root->next;
	root->next = process;
	process->last = root;
	hold->last = process;
	process->next = hold;

	process->state = PROC_RUNNING;

	return process->pid;
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

virtix_proc_t* mk_empty_proc(){
	virtix_proc_t* proc = (virtix_proc_t*) kmalloc(sizeof(virtix_proc_t));
	proc->registers = (registers_t*) kmalloc(sizeof(registers_t));
	proc->registers->cs = 0x08;
	proc->registers->ds = 0x10;
	proc->registers->ss = 0x10;

	return proc;
}
