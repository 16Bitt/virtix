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

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);

size_t strlen(char* str);
int strcmp(char* str1, char* str2);

void panic(char* msg, char* file, unsigned int line);
#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(e) ((e) ? (void) 0 : PANIC("ASSERTION FAILED"))

#endif
