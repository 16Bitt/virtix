#include "common.h"
#include "monitor.h"
#include "isr.h"
#include "virtix_proc.h"
#include "userspace.h"
#include "single.h"

#include "file.h"

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
			regs->eax = single_exit(regs);	//leave return code for parent	
			return;

		default:
			PANIC("bad userspace call");
	}
}

void init_userspace(){
	register_interrupt_handler(0x80, userspace_handler);
}

uint fork(){
	return -1;
}

uint getpid(){
	return current_proc->pid;
}

char** get_env(){
	return (char**) NULL;
}

FILE open(char* path, uint offset){
	return kfopen(path, offset);
}

uint close(FILE fid){
	kfclose(fid);
	return 0;
}

uint read(FILE fid, char* buffer, size_t length){
	return kfread(fid, length, buffer);
}

uint write(FILE fid, char* buffer, size_t length){
	return kfwrite(fid, length, buffer);
}

uint _exit(uint return_code){
	current_proc->pid = 1;
	return 0;
}
