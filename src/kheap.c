#include "common.h"
#include "kheap.h"
#include "monitor.h"

#define KHEAP_INC (BHEAP_SIZE + sizeof(heap_t))

int num_bheaps;

void init_kheaps(int num_heaps){
	int i;
	for(i = 0; i < num_heaps; i++){
		heap_t* curr_heap = (heap_t*) (end + (i * KHEAP_INC));	
		curr_heap->placement_address 	= ((unsigned int) curr_heap) + sizeof(heap_t);
		curr_heap->bottom		= ((unsigned int) curr_heap) + sizeof(heap_t);
		curr_heap->associated		= NULL;
		curr_heap->magic		= KHEAP_MAGIC;
	}

	num_bheaps = num_heaps;
}

heap_t* kbmalloc(char* proc_name){
	int i;
	for(i = 0; i < num_bheaps; i++){
		heap_t* curr_heap = (heap_t*) (end + (i * KHEAP_INC));
		
		if(curr_heap->associated == NULL){
			curr_heap->associated = proc_name;
			return curr_heap;
		}
	}

	return NULL;
}

void kbfree(heap_t* heap){
	ASSERT(heap->magic == KHEAP_MAGIC);
	heap->associated 	= NULL;
	heap->placement_address	= heap->bottom;
}

void* kmalloc(heap_t* heap, size_t size){
	ASSERT(heap->magic == KHEAP_MAGIC);
	
	heap->placement_address &= 0xFFFFF000;
	heap->placement_address += 0x1000;

	ASSERT((heap->placement_address + size) < (heap->bottom + BHEAP_SIZE));

	void* ptr = (void*) heap->placement_address;
	heap->placement_address += size;
	return ptr;
}

void kfree(heap_t* heap, void* ptr){
	ASSERT(heap->magic == KHEAP_MAGIC);	
}

void kheap_list(){
	vga_puts("KHEAPS:");
	int i;
	for(i = 0; i < num_bheaps; i++){
		heap_t* curr_heap = (heap_t*) (end + (i * KHEAP_INC));
		if(curr_heap->associated){
			vga_puts("\t");
			vga_puts(curr_heap->associated);
			vga_puts(":\t");
			vga_puts_hex(curr_heap->bottom);
			vga_puts("\n");
		}
	}
}
