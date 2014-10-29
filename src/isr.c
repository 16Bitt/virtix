#include "common.h"
#include "isr.h"
#include "monitor.h"

isr_t interrupt_handlers[256];

unsigned int* istack_base;

void isr_handler(registers_t regs){
	vga_puts("CAUGHT INTERRUPT: ");
	vga_puts_hex(regs.int_no);
	vga_putc('\n');

	if(interrupt_handlers[regs.int_no]){
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void irq_handler(registers_t regs){
	if(regs.int_no >= 40)
		outb(0xA0, 0x20);

	outb(0x20, 0x20);

	if(interrupt_handlers[regs.int_no]){
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void register_interrupt_handler(unsigned char index, isr_t handler){
	interrupt_handlers[index] = handler;
}

void cli(){
	asm volatile ("cli");
}

void sti(){
	asm volatile ("sti");
}

void hlt(){
	asm volatile ("hlt");
}
