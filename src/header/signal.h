#ifndef SIGNAL_H
#define SIGNAL_H

#define SIGABRT	0
#define SIGFPE	1
#define SIGILL	2
#define SIGINT	3
#define SIGSEGV	4
#define SIGTERM	5

#define NUM_SIGS 6

#define SIG_ERR	(-1)

#include "common.h"
#include "virtix_proc.h"

//Code to raise an exception within a process
void raise(virtix_proc_t* proc, uint signal);

//Code to set an exception handler within a process
void signal(virtix_proc_t* proc, uint signal, uint func);

#endif
