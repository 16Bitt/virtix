#ifndef SIGNAL_H
#define SIGNAL_H

#include "sys/cdefs.h"

__HEADER_START

#define SIGABRT	0
#define SIGFPE	1
#define SIGILL	4
#define SIGINT	8
#define SIGSEGV	16
#define SIGTERM	32

#define SIG_ERR	(-1)

#define SIG_SETMASK 0

#define SIG_DFL	((void*) 1)

int raise(int signal);
int signal(int signal, void (*func)());

typedef int sig_atomic_t;
typedef unsigned long int sigset_t;

int sigemptyset(sigset_t* set);
int sigprocmask(int mask, const sigset_t* set, sigset_t* oset);
int kill(pid_t pid, int signal);

#ifndef pthread_attr_t
	#define pthread_attr_t uint
#endif

union sigval{
	int sival_int;
	void* sival_ptr;
};

struct sigevent{
	int 			sigev_notify;
	int 			sigev_signo;
	union sigval		sigev_value;
	void (*sigev_notify_function)(union sigval);
	pthread_attr_t*		sigev_notify_attributes;
};

__HEADER_END

#endif
