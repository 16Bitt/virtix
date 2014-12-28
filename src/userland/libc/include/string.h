#ifndef STRING_H
#define STRING_H

#include "sys/types.h"

int strcmp(char* str1, char* str2);
int strcpy(char* dest, char* dest);
size_t strlen(char* str);
void memmove(void* dest, void* src, size_t length);
void memcpy(void* dest, void* src, size_t length);
int memcmp(void* addr1, void* addr2, size_t length);
void memset(void* start, uchar value, size_t length);

#endif
