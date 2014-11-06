#include "common.h"
#include "clock.h"
#include "isr.h"
#include "monitor.h"

void timer_handler(registers_t* regs){
	vga_puts("Tick()\n");
}

void start_timer(unsigned int frequency){
	register_interrupt_handler(32, &timer_handler);
	
	unsigned int divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	unsigned char low	= (unsigned char) (divisor & 0xFF);
	unsigned char high	= (unsigned char) ((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
}
