#include "syscall.h"

int syscall_0arg(uint call_no){
	int ret;
	asm volatile ("mov %%ebx, %%eax" : "=a" (call_no));
	asm volatile ("int $0x80");
	asm volatile ("mov %%eax, %%ebx" :: "b" (ret));
	return ret;
}

int syscall_1arg(uint call_no, uint arg1){
	int ret;
	asm volatile ("mov %%eax, %%ebx" : "=a" (arg1));
	asm volatile ("mov %%ebx, %%eax" : "=a" (call_no));
	asm volatile ("int $0x80");
	asm volatile ("mov %%eax, %%ebx" :: "b" (ret));
	return ret;
}

int syscall_2arg(uint call_no, uint arg1, uint arg2){
	int ret;
	asm volatile ("mov %%eax, %%ecx" : "=a" (arg2));
	asm volatile ("mov %%eax, %%ebx" : "=a" (arg1));
	asm volatile ("mov %%ebx, %%eax" : "=a" (call_no));
	asm volatile ("int $0x80");
	asm volatile ("mov %%eax, %%ebx" :: "b" (ret));
	return ret;
}

int syscall_3arg(uint call_no, uint arg1, uint arg2, uint arg3){
	int ret;
	asm volatile ("mov %%eax, %%edx" : "=a" (arg3));
	asm volatile ("mov %%eax, %%ecx" : "=a" (arg2));
	asm volatile ("mov %%eax, %%ebx" : "=a" (arg1));
	asm volatile ("mov %%ebx, %%eax" : "=a" (call_no));
	asm volatile ("int $0x80");	
	asm volatile ("mov %%eax, %%ebx" :: "b" (ret));
	return ret;
}
