#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#define BHEAP_SIZE (2 * 1024 * 1024)
#define KHEAP_MAGIC 0x04206969

extern int num_bheaps;
extern unsigned int end;

typedef struct{
	unsigned int placement_address;
	unsigned int bottom;
	unsigned int magic;
	char* associated;
} heap_t;

heap_t*	kbmalloc(char* proc_name);
void 	kbfree(heap_t* heap);
void* 	kmalloc(heap_t* heap, size_t size);
void 	kfree(heap_t* heap, void* ptr);
void	kheap_list();

#endif
