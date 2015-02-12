#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

void init_keyboard();		//Register the keyboard interrupt handler, etc.
ushort getkey_scancode();	//Poll the handler for the scan code
char getkey_ascii();		//Translate the scan code to ascii and return

#endif
