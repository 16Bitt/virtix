#ifndef WAIT_H
#define WAIT_H

#include "sys/cdefs.h"

__HEADER_START

pid_t wait(int* status);
pid_t waitpid(pid_t pid, int* status, int flags);
pid_t wait3(int* status, int options, void* unused); //Virtix is a bad bad man

#define WNOHANG		1
#define WUNTRACED	2

__HEADER_END

#endif
