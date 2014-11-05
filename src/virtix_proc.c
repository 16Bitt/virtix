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

void scheduler(registers_t* regs){
	memcpy(&current_proc->registers, regs, sizeof(registers_t));

	current_proc = current_proc->next;
	if(current_proc == NULL)
		current_proc = root;

	/*while(current_proc->state == PROC_ASLEEP){
		current_proc = current_proc->next;
		if(current_proc == NULL)
			current_proc = root;
	}*/

	memcpy(regs, &current_proc->registers, sizeof(registers_t));
	if(current_proc != root)
		dump_proc(current_proc);

	switch_page(current_proc->cr3);
}

void force_stub(registers_t* regs){
	sti();
	vga_puts("force_stub(): initializing processes\n");
	memcpy(&hold_root, regs, sizeof(registers_t));
	vga_puts("force_stub(): exiting\n");
}

void force_load_stub(registers_t* regs){
	vga_puts("force_load_stub(): forcibly loading thread\n");
	memcpy(regs, &root->registers, sizeof(registers_t));
}

void init_procs(void* goto_here){
	register_interrupt_handler(31, force_stub);
	asm volatile ("int $31");
	
	vga_puts("init_procs(): captured root\n");

	root = mk_empty_proc();
	root->pid	= pid++;
	root->next	= NULL;
	root->thread	= PROC_ROOT; //Unkillable
	root->state	= PROC_RUNNING;
	memcpy(&root->registers, &hold_root, sizeof(registers_t));
	
	current_proc = root;

	current_proc->name = "ROOT";
	current_proc->cr3 = current_dir;
	
	cli();
	start_timer(1000);
	cli();
	register_interrupt_handler(32, scheduler);
	sti();
}

void kill_proc(unsigned int pid){
	vga_puts("WARN: kill_proc() is dummy stub\n");
}

unsigned int spawn_proc(virtix_proc_t* process){
	vga_puts("swawn_proc(): creating new process\n");
	process->pid = pid++;
	
	process->next = root->next;
	root->next = process;
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
	proc->registers.cs = 0x08;
	proc->registers.ds = 0x10;
	proc->registers.ss = 0x10;
	proc->cr3 = mk_page_dir();

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
	
	vga_puts("\n");
}
