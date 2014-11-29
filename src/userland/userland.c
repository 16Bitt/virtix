#include "common.h"
#include "userspace.h"

uint ret;

uint write(uint fid, char* buffer, size_t length){
	asm volatile ("movl %0, %%ebx" :: "r" (fid));
	asm volatile ("movl %0, %%ecx" :: "r" (buffer));
	asm volatile ("movl %0, %%edx" :: "r" (length));
	asm volatile ("movl $3, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

uint read(uint fid, char* buffer, size_t length){
	asm volatile ("movl %0, %%ebx" :: "r" (fid));
	asm volatile ("movl %0, %%ecx" :: "r" (buffer));
	asm volatile ("movl %0, %%edx" :: "r" (length));
	asm volatile ("movl $4, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

uint close(uint fid){
	asm volatile ("movl %0, %%ebx" :: "r" (fid));
	asm volatile ("movl $5, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

uint _exit(uint return_code){
	asm volatile ("movl %0, %%ebx" :: "r" (return_code));
	asm volatile ("movl $6, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	
	for(;;);

	return ret;
}

uint getpid(){
	asm volatile ("movl $1, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return ret;
}

char** get_env(){
	asm volatile ("movl $2, %eax");
	asm volatile ("int $31");
	asm volatile ("movl %%eax, %0" : "=r" (ret));
	return (char**) ret;
}
