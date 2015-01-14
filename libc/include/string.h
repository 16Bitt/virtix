#ifndef __STRING_H
#define __STRING_H

#include "sys/cdefs.h"

__HEADER_START

//Memory operations
void* memmove(void* dest, void* src, size_t length);
void* memcpy(void* dest, void* src, size_t length);
int memcmp(void* a, void* b, size_t length);
void* memset(void* ptr, int c, size_t n);

//String operations
int strcmp(char* str1, char* str2);
int strncmp(char* str1, char* str2, size_t length);
int strcasecmp(char* str1, char* str2);
int strncasecmp(char* str1, char* str2, size_t length);
char* index(char* s, int c);
char* strchr(char* s, int s);
char* rindex(char* s, int c);
char* strrchr(char* s, int c);
char* strcat(char* dest, char* src);
char* strncat(char* dest, char* src, size_t length);
char* strcpy(char* dest, char* src);
char* strdup(char* s);
size_t strlen(char* s);
char* strstr(char* haystack, char* needle);

__HEADER_END

#endif
