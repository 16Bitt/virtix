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
		case SYS_GETSTDIN:
			regs->eax = current_proc->stdin;
			return;
		case SYS_GETSTDOUT:
			regs->eax = current_proc->stdout;
			return;
		case SYS_GETSTDERR:
			regs->eax = current_proc->stderr;
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
		case SYS_LSEEK:
			regs->eax = klseek(regs->ebx, regs->ecx, regs->edx);
			return;
		case SYS_OPEN:
			regs->eax = open((char*) regs->ebx, regs->ecx);
			return;

		//Modifies the process
		case SYS_EXIT:
			regs->eax = _exit(regs);	//leave return code for parent	
			return;

		default:
			memcpy(&current_proc->registers, regs, sizeof(registers_t));
			dump_proc(current_proc);
			PANIC("bad userspace call");
	}
}

void init_userspace(){
	register_interrupt_handler(31, userspace_handler);
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

uint _exit(registers_t* regs){
	kill_proc(getpid());
	scheduler(regs);
	return 0;
}
