#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

void init_keyboard();		//Register the keyboard interrupt handler, etc.
char getkey_scancode();	//Poll the handler for the scan code
char getkey_ascii();		//Translate the scan code to ascii and return
char translate_scancode(uchar scancode);

//PS/2 Ports
#define KEYBOARD_DATA	0x60
#define KEYBOARD_CMD	0x64
#define KEYBOARD_STAT	0x64

//Key definitions
#define RIGHTSHIFT_DOWN	0x36
#define RIGHTSHIFT_UP	0xB6
#define LEFTSHIFT_DOWN	0x2A
#define LEFTSHIFT_UP	0xAA
#define KBD_Q_DOWN	0x10
#define KBD_W_DOWN	0x11
#define KBD_E_DOWN	0x12
#define KBD_R_DOWN	0x13
#define KBD_T_DOWN	0x14
#define KBD_Y_DOWN	0x15
#define KBD_U_DOWN	0x16
#define KBD_I_DOWN	0x17
#define KBD_O_DOWN	0x18
#define KBD_P_DOWN	0x19
#define KBD_A_DOWN	0x1E
#define KBD_S_DOWN	0x1F
#define KBD_D_DOWN	0x20
#define KDB_F_DOWN	0x21
#define KBD_G_DOWN	0x22
#define KBD_H_DOWN	0x23
#define KBD_J_DOWN	0x24
#define KBD_K_DOWN	0x25
#define KBD_L_DOWN	0x26
#define KBD_Z_DOWN	0x2C
#define KBD_X_DOWN	0x2D
#define KBD_C_DOWN	0x2E
#define KBD_V_DOWN	0x2F
#define KBD_B_DOWN	0x30
#define KBD_N_DOWN	0x31
#define KBD_M_DOWN	0x32
#define KBD_1_DOWN	0x02
#define KBD_2_DOWN	0x03
#define KBD_3_DOWN	0x04
#define KBD_4_DOWN	0x05
#define KBD_5_DOWN	0x06
#define KBD_6_DOWN	0x07
#define KBD_7_DOWN	0x08
#define KBD_8_DOWN	0x09
#define KBD_9_DOWN	0x0A
#define KBD_0_DOWN	0x0B

//PS/2 Statuses
#define KBDSTAT_ACK	0xFA
#define KBDSTAT_RES	0xFE

//PS/2 Commands
#define KBDCMD_SETRATE	0xF3
#define KBDCMD_SETSCAN	0xF0
#define KBDCMD_SETNOUP	0xF9

#endif
