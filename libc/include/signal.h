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
#define SIGTTIN	6
#define SIGTSTP	7
#define SIGTTOU	8
#define SIGCONT	9
#define SIGPIPE	10
#define SIGQUIT	11
#define SIGHUP	12
#define SIGCHLD	13

#define NSIG	14

extern char** sys_siglist;

#define SIG_IGN	(-1)
#define SIG_ERR	(-2)

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

struct sigaction{
	void (*sa_handler)(int);
	sigset_t 	sa_mask;
	int 		sa_flags;
	void (*sa_sigaction)(int, void*, void*);
};

__HEADER_END

#endif
