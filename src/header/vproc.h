#ifndef VPROC_H
#define VPROC_H

typedef struct vprocess{
		//Register status
		unsigned int	pc_;
		int		sp_;
		unsigned int	fp_;
		
		//User priveledge
		unsigned int	uid;

		//Thread identity
		unsigned int	pid;
		
		//Code and data segments
		unsigned int*	code_;
		void*		data_;
		unsigned int*	stack_;

		unsigned int	code_size;
		unsigned int	data_size;
		unsigned int	stack_size;

		//Parent process
		struct vprocess* parent;
} vproc_t;

typedef unsigned int var;

//This will make cleaner code since we are always referring to the
//values of the current process
#define pc	current->pc_
#define sp	current->sp_
#define fp	current->fp_
#define code	current->code_
#define data	current->data_
#define stack	current->stack_

//Update process
int tick(vproc_t* process);

//spawn new process
vproc_t* new_proc(int* new_code, int code_size, int data_size, int stack_size, int entry_point, int uid, int pid);

//Clean up after finished process. Make sure process is finished,
//or that VM_KILL has been sent and the process has been given
//50 tick()'s to finish after the signal
void cleanup_proc(vproc_t* current);

//Signals a clean exit
#define VM_OK		(1)
#define VM_NOT_OK	(0xFFFFFFFF)

#endif
