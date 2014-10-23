#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#define KHEAP_MAGIC 0x04206969

#define MEM_END 0x8000000
#undef data

extern unsigned int end;

typedef struct heap_type{
	unsigned int magic;		//Confirms validity
	unsigned int size;		//Size of heap
	
	void* data;			//Location of data (addr + HEAP_S)

	struct heap_type* parent;	//Previous heap
	struct heap_type* child1;	//left branch
	struct heap_type* child2;	//right branch

	int free;			//Is this open?
} heap_t;

#define HEAP_S (sizeof(heap_t))

//void init_kheap();			//Initialize heap globally

void* kmalloc(unsigned int size);	//straight kmalloc
void* kmalloc_a(unsigned int size);	//page aligned kmalloc
void kfree(void* ptr);			//Free unused memory

//heap_t* find_first_free(heap_t* parent, unsigned int size);	//Find first available node

//void heap_split(heap_t* parent, unsigned int size);		//Subdivide a parent into two heaps with a data pool of
//void heap_collapse(heap_t* parent);				//Converge children and parent into one big heap

#endif
