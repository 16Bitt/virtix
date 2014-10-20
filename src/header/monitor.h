#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

void vga_putc(char c);
void vga_clear();
void vga_puts(char* s);
void vga_puts_hex(int n);

#endif
