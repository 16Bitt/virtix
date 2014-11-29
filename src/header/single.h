#ifndef SINGLE_H
#define SINGLE_H

#include "common.h"
#include "virtix_proc.h"
#include "isr.h"

unsigned int sp_child(virtix_proc_t* child);	//Spawn "child" in single process mode
void enter_userspace(virtix_proc_t* proc);	//Force CPU to run in user mode in proc 'force'
void single_yield(virtix_proc_t* proc, registers_t* regs); //yield execution to another process
unsigned int single_exit(registers_t* regs);		//Leave process

extern void hard_usermode();			//Assembly hardloading stub

#endif
