#ifndef __STDIO_H
#define __STDIO_H

#include "sys/cdef.h"

__HEADER_START

typedef FILE unsigned int;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

//General file management
FILE* fopen(char* name, char* mode);
int fclose(FILE* f);

//File output
int fprintf(FILE* f, char* fmt, ...);
int fputs(FILE* f, char* str);

//Terminal Output
int putchar(char c);
int puts(char* str);
int printf(char* fmt, ...);

__HEADER_END

#endif
