#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "signal.h"
#include "virtix_proc.h"

void raise(virtix_proc_t* proc, uint signal){
	PANIC("No signal handling per process");
}

void signal(virtix_proc_t* proc, uint signal, uint func){
	WARN("does not support signals... Yet")
}
