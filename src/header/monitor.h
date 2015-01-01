#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void vga_putc(char c);
void vga_clear();
void vga_puts(char* s);
void vga_puts_hex(int n);
void vga_set_fg(uchar color);
void vga_set_bg(uchar color);
void vga_blink();
void vga_reset();

#define DISP(name,val)	vga_puts((name));\
			vga_puts(":\t");\
			vga_puts_hex((val));\
			vga_puts("\n");

#define NOTIFY(str)	vga_set_fg(CYAN);\
			vga_puts((char*) __FUNCTION__);\
			vga_puts("(): ");\
			vga_reset();\
			vga_puts((str));\
			vga_puts("\n");

#define WARN(str)	vga_set_fg(RED);\
			vga_puts((char*) __FUNCTION__);\
			vga_puts("(): ");\
			vga_reset();\
			vga_puts((str));\
			vga_puts("\n");

#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED		4
#define MAGENTA		5
#define BROWN		6
#define LIGHT_GREY	7
#define DARK_GREY	8
#define LIGHT_BLUE	9
#define LIGHT_GREEN	10
#define LIGHT_CYAN	11
#define LIGHT_RED	12
#define LIGHT_MAGENTA	13
#define LIGHT_BROWN	14
#define WHITE		15

#endif
