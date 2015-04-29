#ifndef TTY_H
#define TTY_H

#include "common.h"

//This uses the VGA text mode as a vt

#define TTY_HEIGHT	25
#define TTY_WIDTH	80

extern char tty_key;

void tty_init();

void tty_clear();
void tty_reset();
void tty_putc(char c);
char tty_getc();

#endif
