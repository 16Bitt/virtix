#ifndef KSTUB_H
#define KSTUB_H

#include "common.h"
#include "vproc.h"

//Services a system call to the current process
var kservice(vproc_t* current, var option);

//Maps a vproc to the kstub handler
void map_kstub(vproc_t* stub);

#endif