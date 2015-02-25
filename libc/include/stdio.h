#ifndef __STDIO_H
#define __STDIO_H

#include "sys/cdefs.h"

__HEADER_START

typedef unsigned int FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

//General file management
FILE* fopen(char* name, char* mode);
int fclose(FILE* f);

//File output
int fprintf(FILE* f, const char* fmt, ...);
int fprint(FILE* f, char* s);
int fputs(FILE* f, char* str);
int fputchar(FILE* f, char c);

//Terminal Output
int putchar(char c);
int puts(char* str);
int printf(const char* fmt, ...);
int print(char* s);

__HEADER_END

#endif
