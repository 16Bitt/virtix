#include "common.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "isr.h"

virtix_proc_t* pstack[10];
int psp	= 0;

unsigned int sp_child(){
	return (unsigned int) -1;
}

void enter_userspace(){
	hard_usermode();
}

void usermode_loader(){
	vga_puts("You made it to the usermode party!\n");
	cli();
	hlt();
}
