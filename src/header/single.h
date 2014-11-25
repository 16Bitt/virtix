#ifndef SINGLE_H
#define SINGLE_H

#include "common.h"
#include "virtix_proc.h"
#include "isr.h"

unsigned int sp_child(virtix_proc_t* child);	//Spawn "child" in single process mode
void enter_userspace(virtix_proc_t* proc);	//Force CPU to run in user mode in proc 'force'

void sp_int_handle(registers_t* regs);		//This is the stub for single process syscalls
void usermode_loader();				//Called by hardloading user mode

extern void hard_usermode();			//Assembly hardloading stub

#endif
