#ifndef COMMON_H
#define COMMON_H

typedef unsigned int 	u32int;
typedef int		s32int;
typedef unsigned short 	u16int;
typedef short 		s16int;
typedef unsigned char	u8int;
typedef char		s8int;
typedef int		size_t;
typedef unsigned int	uint;
typedef unsigned char	uchar;
typedef unsigned short	ushort;

#define NULL 0
#define FALSE 0
#define TRUE (!FALSE)

typedef int bool;
#define false 0
#define true (!false)

void outb(unsigned short port, unsigned char value);
void outw(ushort port, ushort value);
unsigned char inb(unsigned short port);
ushort inw(ushort port);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);

size_t strlen(char* str);
int strcmp(char* str1, char* str2);

void dump_struct(void* structure, size_t bytes);

void panic(char* msg, char* file, unsigned int line);
#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(e) ((e) ? (void) 0 : PANIC("ASSERTION FAILED:\n\n" #e))

#endif
