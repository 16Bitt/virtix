#ifndef VM_FILE_H
#define VM_FILE_H

#include "vproc.h"

#define VFILE_MAGIC	0x80084545
#define VFILE_INVALID	0xFFFFFFFF

typedef struct{
	//validation number
	var magic;

	//interrupt vectors
	var code_start;
	var exit_start;

	//load paramaters
	var data_size;
	var code_size;
	var stack_size;
	var permissions;
} __attribute__((packed)) vfile_t;

//loads a process for a particular user
vproc_t* load_vfile(vfile_t* file, var uid, var pid);

//checks for VM_MAGIC
var validate_vfile(vfile_t* file);

#endif
