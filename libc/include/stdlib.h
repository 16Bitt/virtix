#ifndef __STDLIB_H
#define __STDLIB_H

#include "sys/cdefs.h"

__HEADER_START

void* malloc(size_t s);
void free(void* addr);

long long int strtoll(const char* str, char** endptr, int base);

__HEADER_END

#endif
