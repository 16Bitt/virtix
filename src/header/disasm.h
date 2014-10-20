#ifndef DISASM_H
#define DISASM_H
#include "vproc.h"

void disasm(vproc_t* proc, var address);
void stack_dump(vproc_t* proc);
void data_dump(vproc_t* proc);
void regs_dump(vproc_t* proc);

#endif
