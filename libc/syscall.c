#include "syscall.h"

int syscall_0arg(uint call_no){
	int ret;

	asm volatile ("movl %0, %%eax" :: "b" (call_no));
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

int syscall_1arg(uint call_no, uint arg1){
	int ret;
	asm volatile ("movl %0, %%ebx" :: "c" (arg1));
	asm volatile ("movl %0, %%eax" :: "c" (call_no));
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

int syscall_2arg(uint call_no, uint arg1, uint arg2){
	int ret;
	asm volatile ("movl %0, %%ecx" :: "d" (arg2));
	asm volatile ("movl %0, %%ebx" :: "d" (arg1));
	asm volatile ("movl %0, %%eax" :: "d" (call_no));
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

int syscall_3arg(uint call_no, uint arg1, uint arg2, uint arg3){
	int ret;
	asm volatile ("movl %0, %%eax" :: "d" (call_no));
	asm volatile ("pushl %eax");
	asm volatile ("movl %0, %%edx" :: "a" (arg3));
	asm volatile ("movl %0, %%ecx" :: "a" (arg2));
	asm volatile ("movl %0, %%ebx" :: "a" (arg1));
	asm volatile ("popl %eax");
	asm volatile ("int $31");	
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

int syscall_4arg(uint call_no, uint arg1, uint arg2, uint arg3, uint arg4){
	int ret;
	asm volatile ("movl %0, %%eax" :: "d" (call_no));
	asm volatile ("pushl %eax");
	asm volatile ("movl %0, %%esi" :: "a" (arg4));
	asm volatile ("movl %0, %%edx" :: "a" (arg3));
	asm volatile ("movl %0, %%ecx" :: "a" (arg2));
	asm volatile ("movl %0, %%ebx" :: "a" (arg1));
	asm volatile ("popl %eax");
	asm volatile ("int $31");	
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}
