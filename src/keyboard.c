#include "common.h"
#include "keyboard.h"
#include "isr.h"
#include "monitor.h"
#include "clock.h"
#include "tty.h"

uchar last_key_pressed = 0;
bool shift_on = FALSE;
int keys_pressed;

void keyboard_handler(registers_t* regs){
	uchar hold = last_key_pressed;
	last_key_pressed = translate_scancode(inb(KEYBOARD_DATA));
	keys_pressed++;

	if(last_key_pressed == 0){
		last_key_pressed = hold;
		hold = 0xFF;	//This was a dud, ignore
	}
	
	if(hold != 0xFF)	//ignore the dud
		tty_putc(last_key_pressed);
}

uint keyboard_getc(){
	uchar hold = last_key_pressed;
	last_key_pressed = 0;
	return hold;
}

void init_keyboard(){
	register_interrupt_handler(33, &keyboard_handler);
	keys_pressed = 1;
}

#define BETWEEN(n, l, u) (((n) >= (l)) && ((n) <= (u)))

char translate_scancode(uchar scancode){
	if(shift_on == FALSE){
		char* qwertyuiop = "qwertyuiop";
		if(BETWEEN(scancode, KBD_Q_DOWN, KBD_P_DOWN))
			return qwertyuiop[scancode - KBD_Q_DOWN];
		
		char* asdfghjkl = "asdfghjkl";
		if(BETWEEN(scancode, KBD_A_DOWN, KBD_L_DOWN))
			return asdfghjkl[scancode - KBD_A_DOWN];

		
		char* zxcvbnm = "zxcvbnm";
		if(BETWEEN(scancode, KBD_Z_DOWN, KBD_M_DOWN))
			return zxcvbnm[scancode - KBD_Z_DOWN];
		
		char* numbers = "1234567890";
		if(BETWEEN(scancode, KBD_1_DOWN, KBD_0_DOWN))
			return numbers[scancode - KBD_1_DOWN];
	}

	else{
		char* up_qwertyuiop = "QWERTYUIOP";
		if(BETWEEN(scancode, KBD_Q_DOWN, KBD_P_DOWN))
			return up_qwertyuiop[scancode - KBD_Q_DOWN];
		
		char* up_asdfghjkl = "ASDFGHJKL";
		if(BETWEEN(scancode, KBD_A_DOWN, KBD_L_DOWN))
			return up_asdfghjkl[scancode - KBD_A_DOWN];

		
		char* up_zxcvbnm = "ZXCVBNM";
		if(BETWEEN(scancode, KBD_Z_DOWN, KBD_M_DOWN))
			return up_zxcvbnm[scancode - KBD_Z_DOWN];
		
		char* up_numbers = "!@#$%^&*()";
		if(BETWEEN(scancode, KBD_1_DOWN, KBD_0_DOWN))
			return up_numbers[scancode - KBD_1_DOWN];
	}

	switch(scancode){
		case RIGHTSHIFT_DOWN:
		case LEFTSHIFT_DOWN:
			shift_on = TRUE;
			break;
		
		case RIGHTSHIFT_UP:
		case LEFTSHIFT_UP:
			shift_on = FALSE;
			break;

		default:
			break;
	}

	return 0;
}
