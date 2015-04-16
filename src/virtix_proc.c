#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "clock.h"
#include "isr.h"
#include "virtix_page.h"
#include "single.h"
#include "kheap.h"
#include "file.h"

unsigned int pid = 0;
unsigned int gid = 0;

virtix_proc_t* root;
virtix_proc_t* current_proc = NULL;

registers_t hold_root;

extern unsigned int stack_hold;

void scheduler(registers_t* regs){
	memcpy(&current_proc->registers, regs, sizeof(registers_t));
	
	//NOTIFY("task switch");

	virtix_clock(regs);

	current_proc = current_proc->next;
	if(current_proc == NULL){
		current_proc = root;
	}

	while(current_proc->state == PROC_ASLEEP){
		current_proc = current_proc->next;
		if(current_proc == NULL)
			current_proc = root;
	}

	memcpy(regs, &current_proc->registers, sizeof(registers_t));
	switch_vpage_dir(current_proc->cr3);
}

void init_procs(virtix_proc_t* proc){
	root		= proc;
	root->pid	= pid++;
	root->next	= NULL;
	root->thread	= PROC_ROOT; //Unkillable
	root->state	= PROC_RUNNING;
	
	current_proc = root;
	register_interrupt_handler(32, scheduler);
	enter_userspace(proc);
}

void kill_proc(unsigned int pid){
	virtix_proc_t* proc = pid_to_proc(pid);

	if(proc == PID_NOT_FOUND)
		PANIC("can't kill unknown PID");
	
	kfflush(proc->stdout);
	kfflush(proc->stderr);
	proc->state = PROC_ASLEEP;

	if(proc->parent != NULL){
		NOTIFY("Child had parent (!!!)");
		wake_proc(proc->parent->pid);
	}
}

unsigned int spawn_proc(virtix_proc_t* process){
	process->pid = pid++;
	
	process->next = root->next;
	root->next = process;
	process->state = PROC_RUNNING;
	
	process->gid = process->parent->pid + 1;

	return process->pid;
}

//Find out if a child process has died
int wait_gid(uint gid, int* status){
	virtix_proc_t* i = current_proc->next;

	while(i != NULL){
		if(i->gid == gid)
			if(i->state == PROC_ASLEEP){
				i->gid = 0;		//Set gid to 0 so we don't rescan
				*status = i->registers.eax;
				return WAIT_KILLED;
			}
			
		i = i->next;
	}

	return WAIT_OKAY;
}

//Find out if a process has died
int wait_pid(uint pid, int* status){
	virtix_proc_t* i = current_proc->next;

	while(i != NULL){
		if(i->pid == pid)
			if(i->state == PROC_ASLEEP){
				*status = i->registers.eax;
				return WAIT_KILLED;
			}
			else
				return WAIT_OKAY;
		i = i->next;
	}

	return WAIT_ERROR;
}

void susp_proc(unsigned int pid){
	virtix_proc_t* proc = pid_to_proc(pid);
	
	if(proc == PID_NOT_FOUND){
		WARN("couldn't find PID for suspension");
		return;
	}

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
	WARN("dummy stub");
	return pid++;
}

virtix_proc_t* pid_to_proc(unsigned int pid){
	virtix_proc_t* proc = root;

	while(proc != NULL){
		if(proc->pid == pid)
			return proc;

		proc = proc->next;
	}

	return PID_NOT_FOUND;
}

virtix_proc_t* mk_empty_proc(){
	virtix_proc_t* proc = (virtix_proc_t*) kmalloc_a(sizeof(virtix_proc_t));
	proc->registers.cs = 0x1B;
	proc->registers.ds = 0x23;
	proc->registers.ss = 0x23;
	proc->cr3 = mk_vpage_dir();
	
	proc->brk	= 0x50000000;

	int i;
	for(i = 0; i < 1024; i++)
		proc->cr3->tables[i] = root_vpage_dir->tables[i];

	return proc;
}

void dump_proc(virtix_proc_t* proc){
	vga_puts("Process '");
	vga_puts(proc->name);
	vga_puts("' with PID ");
	vga_puts_hex(proc->pid);
	vga_puts(" before last call to scheduler:");

	vga_puts("\nEAX=");
	vga_puts_hex(proc->registers.eax);
	vga_puts("\tEBX=");
	vga_puts_hex(proc->registers.ebx);
	vga_puts("\tECX=");
	vga_puts_hex(proc->registers.ecx);
	vga_puts("\tEDX=");
	vga_puts_hex(proc->registers.edx);

	vga_puts("\nEDI=");
	vga_puts_hex(proc->registers.edi);
	vga_puts("\tESI=");
	vga_puts_hex(proc->registers.esi);
	vga_puts("\tEBP=");
	vga_puts_hex(proc->registers.ebp);
	vga_puts("\tESP=");
	vga_puts_hex(proc->registers.esp);

	vga_puts("\nDS =");
	vga_puts_hex(proc->registers.ds);
	vga_puts("\tCS =");
	vga_puts_hex(proc->registers.cs);
	vga_puts("\tSS =");
	vga_puts_hex(proc->registers.ss);
	vga_puts("\tUSP=");
	vga_puts_hex(proc->registers.useresp);
	
	vga_puts("\n*EFLAGS*=");
	vga_puts_hex(proc->registers.eflags);
	vga_puts("\t*EIP*=");
	vga_puts_hex(proc->registers.eip);

	vga_puts("\t*CR3*=");
	vga_puts_hex((unsigned int) proc->cr3);
	vga_puts("/");

	unsigned int cr3;
	asm volatile ("mov %%cr3, %0" : "=r" (cr3));
	vga_puts_hex(cr3);
	
	vga_puts("\n");
}
