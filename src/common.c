//Port IO, for interfacing non-memory mapped hardware

#include "common.h"
#include "monitor.h"
#include "isr.h"

void outb(u16int port, u8int value){
	asm volatile("outb %1, %0"::"dN"(port), "a"(value));
}

void outw(ushort port, ushort value){
	asm volatile("outw %1, %0"::"dN"(port), "a"(value));
}

u8int inb(u16int port){
	u8int ret;
	asm volatile("inb %1, %0":"=a"(ret):"dN"(port));
	return ret;
}

u16int inw(u16int port){
	u16int ret;
	asm volatile("inw %1, %0":"=a"(ret):"dN"(port));
	return ret;
}

//Basic memory movement routines
void* memset(void* s, int c, size_t n){
	char* str = (char*) s;
	
	int i;
	for(i = 0; i < n; i++)
		str[i] = (char) c;

	return s;
}

void* memcpy(void* dest, const void* src, size_t n){
	char* dest_str = (char*) dest;
	char* src_str = (char*) src;

	int i;
	for(i = 0; i < n; i++)
		dest_str[i] = src_str[i];

	return dest;
}

size_t strlen(char* str){
	size_t len = 0;
	while(str[len])
		len++;
	
	return len;
}

int strcmp(char* str1, char* str2){
	if(strlen(str1) != strlen(str2))
		return -1;

	size_t end = strlen(str1);
	int i;
	for(i = 0; i < end; i++)
		if(str1[i] != str2[i])
			return -1;

	return 0;
}

void panic(char* msg, char* file, unsigned int line){
	vga_set_fg(RED);
	vga_set_bg(WHITE);
	
	vga_puts("\nPANIC: ");
	vga_puts(file);
	vga_puts(" on line ");
	vga_puts_hex(line);

	vga_puts(" because ");
	vga_puts(msg);

	int i;
	for(;;)
		i++;
}

