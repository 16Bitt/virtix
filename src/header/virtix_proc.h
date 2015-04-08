#ifndef VIRTIX_PROC_H
#define VIRTIX_PROC_H

#include "common.h"
#include "isr.h"
#include "virtix_page.h"

typedef struct {
	unsigned int text;
	unsigned int bss;
	unsigned int data;
	unsigned int stack;
} mmap_t;

typedef struct virtix_proc {
	vpage_dir_t* cr3;		//Memory pages
	registers_t registers;		//Process state

	unsigned int pid;		//Process integer ID
	char* name;			//Process/program name
		
	int state;			//Sleeping | running
	int thread;			//Whether process is thread
	
	uint stdin;			//Current streams
	uint stdout;
	uint stderr;
	
	uint brk;

	struct virtix_proc* parent;	//If child, this will be set
	struct virtix_proc* next;	//Next process
} virtix_proc_t;

void scheduler(registers_t* regs);

void kill_proc(unsigned int pid);							//End a process
unsigned int spawn_proc(virtix_proc_t* process);			//Start a process
void susp_proc(unsigned int pid);							//Suspend a process
void wake_proc(unsigned int pid);							//Wake a process back up
unsigned int child_proc(virtix_proc_t* process, unsigned int pid);	//Sleep the process
unsigned int fork_proc(unsigned int pid);					//Fork the process

virtix_proc_t* pid_to_proc(unsigned int pid);				//Get a process from a PID

void init_procs(virtix_proc_t* proc);							//maps a process handler to the PIT

virtix_proc_t* mk_empty_proc();								//Make an empty (initialized) process structure

void dump_proc(virtix_proc_t* proc);

extern virtix_proc_t* current_proc;

extern unsigned int stack_hold;

#define PID_NOT_FOUND	((virtix_proc_t*) 0xFFFFFFFF)

#define PROC_RUNNING	0
#define PROC_ASLEEP	1

#define PROC_THREAD	0
#define PROC_PROC	1
#define PROC_ROOT	2

#endif
