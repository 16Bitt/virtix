#ifndef EXEC_H
#define EXEC_H

#include "common.h"

uint kexec(char* path);
uint kexec_pipes(char* path, char* stdin, char* stdout, char* stderr);
uint uexec(char* path);	//DO NOT CALL DIRECTLY, only for interrupt use

#endif
