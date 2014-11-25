#ifndef ISR_H
#define ISR_H

#include "common.h"

extern unsigned int* istack_base;
extern unsigned int stack_hold;

typedef struct registers{
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int int_no, err_code;
	u32int eip, cs, eflags, useresp, ss;
}registers_t;

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(unsigned char index, isr_t handler);
void cli();
void sti();
void hlt();

void register_default_handlers();

#endif
