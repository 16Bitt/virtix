#ifndef KTHREAD_H
#define KTHREAD_H

#include "common.h"
#include "virtix_proc.h"

//Make a kernel thread at addr with name "name"
void mk_kthread(char* name, void* addr);

//Hang a kthread
void kthread_hlt();

//Exit kthread
void kthread_exit();

#endif