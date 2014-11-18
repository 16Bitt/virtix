#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void vga_putc(char c);
void vga_clear();
void vga_puts(char* s);
void vga_puts_hex(int n);

#define DISP(name,val)	vga_puts((name));\
			vga_puts(":\t");\
			vga_puts_hex((val));\
			vga_puts("\n");

#endif
