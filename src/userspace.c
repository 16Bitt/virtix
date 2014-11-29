#include "common.h"
#include "monitor.h"
#include "isr.h"
#include "virtix_proc.h"
#include "userspace.h"

uint c_err;

void userspace_handler(registers_t* regs){
	switch(regs->eax){

		//One liners
		case SYS_FORK:
			regs->eax = fork();
			return;
		case SYS_GETPID:
			regs->eax = getpid();
			return;
		case SYS_GETENV:
			regs->eax = (uint) get_env();
			return;
		
		//Need arguments
		case SYS_WRITE:
			regs->eax = write(regs->ebx, (char*) regs->ecx, (size_t) regs->edx);
			return;
		case SYS_READ:
			regs->eax = read(regs->ebx, (char*) regs->ecx, (size_t) regs->edx);
			return;
		case SYS_CLOSE:
			regs->eax = close(regs->ebx);
			return;

		//Modifies the process
		case SYS_EXIT:
			regs->eax = _exit(regs->ebx);
			return;

		default:
			PANIC("bad userspace call");
	}
}

void init_userspace(){
	register_interrupt_handler(31, userspace_handler);
}

uint fork(){
	return 0;
}

uint getpid(){
	return current_proc->pid;
}

char** get_env(){
	return (char**) NULL;
}

uint write(uint fid, char* buffer, size_t length){
	if(fid == 0){
		uint i;
		for(i = 0; i < length; i++)
			vga_putc(buffer[i]);
		
		return 0;
	}

	return (uint) -1;
}

uint read(uint fid, char* buffer, size_t length){
	return (uint) -1;
}

uint close(uint fid){
	return 0;
}

uint _exit(uint return_code){
	current_proc->pid = 1;
	return 0;
}
