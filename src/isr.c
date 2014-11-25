#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "virtix_page.h"

isr_t interrupt_handlers[256];

unsigned int* istack_base;

void isr_handler(registers_t* regs){
	vga_puts("CAUGHT INTERRUPT: ");
	vga_puts_hex(regs->int_no);
	vga_putc('\n');

	if(interrupt_handlers[regs->int_no]){
		isr_t handler = interrupt_handlers[regs->int_no];
		handler(regs);
	}
}

void irq_handler(registers_t* regs){
	if(regs->int_no >= 40)
		outb(0xA0, 0x20);

	outb(0x20, 0x20);

	if(interrupt_handlers[regs->int_no]){
		isr_t handler = interrupt_handlers[regs->int_no];
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

void ex_double_fault(registers_t* regs){
	PANIC("double fault");
}

void ex_div_by_zero(registers_t* regs){
	PANIC("division by zero");
}

void ex_bad_opcode(registers_t* regs){
	PANIC("bad opcode");
}

void ex_bad_tss(registers_t* regs){
	PANIC("bad TSS");
}

void ex_bad_simd(registers_t* regs){
	PANIC("fatal simd error");
}

void ex_overflow(registers_t* regs){
	vga_puts("\n!OVERFLOW\n");
}

void register_default_handlers(){
	register_interrupt_handler(0, ex_div_by_zero);
	register_interrupt_handler(4, ex_overflow);
	register_interrupt_handler(8, ex_double_fault);
	register_interrupt_handler(6, ex_bad_opcode);
	register_interrupt_handler(0xA, ex_bad_tss);
	register_interrupt_handler(0x13, ex_bad_simd);
}
