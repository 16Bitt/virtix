#ifndef EXEC_H
#define EXEC_H

#include "common.h"

uint kexec(char* path);
uint kexec_pipes(char* path, char* stdin, char* stdout, char* stderr);

#endif
