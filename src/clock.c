#include "common.h"
#include "clock.h"
#include "isr.h"
#include "monitor.h"

uint virtix_time = 0;
uint time_div = 0;

void virtix_clock(registers_t* regs){
	time_div++;
	if(time_div == 100){
		time_div = 0;
		virtix_time++;
	}
}

void start_timer(unsigned int frequency, clock_handler_t handler){
	register_interrupt_handler(32, handler);
	
	unsigned int divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	unsigned char low	= (unsigned char) (divisor & 0xFF);
	unsigned char high	= (unsigned char) ((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
}

void init_virtix_clock(){
	start_timer(1, virtix_clock);
}

uint get_seconds(){
	return virtix_time;
}
