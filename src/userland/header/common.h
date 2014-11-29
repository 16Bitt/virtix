#ifndef COMMON_H
#define COMMON_H

typedef unsigned int 	u32int;
typedef int		s32int;
typedef unsigned short 	u16int;
typedef short 		s16int;
typedef unsigned char	u8int;
typedef char		s8int;
typedef int		size_t;

#define NULL 0
#define FALSE 0
#define TRUE (!FALSE)

typedef int bool;
#define false 0
#define true (!false)


void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);

size_t strlen(char* str);
int strcmp(char* str1, char* str2);

#endif
