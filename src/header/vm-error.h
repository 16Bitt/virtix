#ifndef VM_ERROR_H
#define VM_ERROR_H
#include "vproc.h"

#define VMERR	(-1)

//Runtime error
int error(char* message, vproc_t* process, int e_offs);
//Internal error
void e_fatal(char* message, char* file, int line);

#define ASSERT(cond,msg) (cond)?(void)0:e_fatal(msg, __FILE__, __LINE__);

#endif
