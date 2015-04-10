#ifndef SIGNAL_H
#define SIGNAL_H

#include "sys/cdefs.h"

__HEADER_START

#define SIGABRT	0
#define SIGFPE	1
#define SIGILL	2
#define SIGINT	3
#define SIGSEGV	4
#define SIGTERM	5

#define SIG_ERR	(-1)

int raise(int signal);
int signal(int signal, void (*func)());

__HEADER_END

#endif
