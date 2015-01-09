#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#define KHEAP_MAGIC	0x04206969
#define KHEAP_MAGIC2	0xDEADBEEF

#define KHEAP_END	0xFFFFDEAD

#define MEM_END 0x8000000

extern unsigned int end;

typedef struct{
	uint magic;
	bool free;
	uint size;
	uint magic2;
} heap_header_t;

typedef struct{
	uint magic;
	uint size;
	uint magic2;
} heap_footer_t;

#define HEAP_S		(sizeof(heap_header_t))
#define HEAP_TOTAL	(sizeof(heap_footer_t) + HEAP_S)
#define HEAP_MINIMUM	1
#define HEAP_FIND_SIZE	(HEAP_TOTAL + HEAP_MINIMUM)

void init_kheap();			//Initialize heaps globally

void* fmalloc(uint size);
void* kmalloc(unsigned int size);	//straight kmalloc
void* kmalloc_a(unsigned int size);	//page aligned kmalloc
void kfree(void* ptr);			//Free unused memory

void* umalloc(size_t size);		//Allocate in userspace
void ufree(void* address);		//Free in userspace

void init_heap(heap_header_t* heap, size_t size);

#define KHEAP_SIZE 0xFFFFF
#define UHEAP_SIZE 0xFFFFF

#endif
