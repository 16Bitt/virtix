#ifndef CLOCK_H
#define CLOCK_H

#include "common.h"
#include "isr.h"

typedef void (*clock_handler_t) (registers_t* regs);

void start_timer(unsigned int frequency, clock_handler_t handler);
void init_virtix_clock();
uint get_seconds();

#endif
