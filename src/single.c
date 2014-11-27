#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "isr.h"

virtix_proc_t* pstack[10];
int psp	= 0;

//These values are read by hardload function
unsigned int hl_cr3;
unsigned int hl_eip;
unsigned int hl_esp;

unsigned int sp_child(){
	return (unsigned int) -1;
}

void enter_userspace(virtix_proc_t* proc){
	hl_eip = proc->registers.eip;
	switch_vpage_dir(proc->cr3);
	hard_usermode();
}

void usermode_loader(){
	//vga_puts("You made it to the usermode party!\n");
	hlt();
}
