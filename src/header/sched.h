#ifndef SCHED_H
#define SCHED_H

#include "vproc.h"

//Number of processes
#define SCHED_SIZE 16

//Public list of vprocs that are running
extern vproc_t** vproc_list;

//Initialize memory for scheduler
void init_sched();

//Runs all current processes in process list for one iteration
var tick_all();

//Add and remove processes
var mk_vproc(vproc_t* proc);
var rm_vproc(vproc_t* proc);

#endif
