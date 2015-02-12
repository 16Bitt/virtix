#include "common.h"
#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

void keyboard_handler(registers_t* regs){
	NOTIFY("key pressed on keyboard")
	inb(0x60);
}

void init_keyboard(){
	register_interrupt_handler(33, &keyboard_handler);
}
