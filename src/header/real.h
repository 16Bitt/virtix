#ifndef REAL_H
#define REAL_H

#include "common.h"

//Run a 16Bit stub in real mode at 0x7C00 and then return to pmode
void run_stub16(char* file);

#endif
