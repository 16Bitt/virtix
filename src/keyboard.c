#include "common.h"
#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

uchar last_key = 0;
bool shift_on = FALSE;
int keys_pressed;

void keyboard_handler(registers_t* regs){
	last_key = translate_scancode(inb(KEYBOARD_DATA));
	keys_pressed++;
}

void init_keyboard(){
	register_interrupt_handler(33, &keyboard_handler);
	keys_pressed = 1;
}

#define BETWEEN(n, l, u) (((n) >= (l)) && ((n) <= (u)))

char translate_scancode(char scancode){
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

	return 0;
}
