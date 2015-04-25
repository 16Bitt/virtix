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
int fputc(FILE* f, char c);

//Terminal Output
int putchar(char c);
int puts(char* str);
int printf(const char* fmt, ...);
int print(char* s);

//File input
int fgetc(FILE* f);
int fgetchar(FILE* f);
char* fgets(char* s, int size, FILE* file);
char* gets(char* s);

__HEADER_END

#endif
