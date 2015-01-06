#include "common.h"
#include "kheap.h"
#include "monitor.h"

unsigned int placement_address;

heap_header_t* kheap = NULL;
heap_header_t* uheap = NULL;

void init_kheap(){
	end = (unsigned int) &end;
	placement_address = end;

	kheap = (heap_header_t*) fmalloc(KHEAP_SIZE);
	init_heap(kheap, KHEAP_SIZE);
}

void* fmalloc(unsigned int size){
	ASSERT(placement_address + size < MEM_END);
	unsigned int hold = placement_address;
	memset((void*) hold, 0, size);
	placement_address += size;
	return (void*) hold;
}

void* kmalloc_a(unsigned int size){
	ASSERT(((placement_address & 0xFFFFF000) + 0x1000) + size < MEM_END);
	placement_address &= 0xFFFFF000;
	placement_address += 0x1000;

	unsigned int hold = placement_address;
	placement_address += size;
	
	return (void*) hold;
}

heap_header_t* find_sized_heap(heap_header_t* heap, size_t size){
	while((heap->free != TRUE) || (heap->size < size + HEAP_FIND_SIZE)){
		ASSERT(heap->magic == KHEAP_MAGIC);
		ASSERT(heap->magic2 == KHEAP_MAGIC2);
		heap_footer_t* foot = (heap_footer_t*) ((uint) heap + HEAP_S + heap->size);
		ASSERT(foot->magic == KHEAP_MAGIC);
		ASSERT(foot->magic2 == KHEAP_MAGIC2);
		
		if(foot->size == KHEAP_END)
			PANIC("out of heap space");
		
		if(foot->size != heap->size)
			PANIC("corrupted heap footer or header");
		

		heap = (heap_header_t*) ((uint) foot + sizeof(heap_footer_t));
	}

	return heap;
}

void split_heap(heap_header_t* heap, size_t size){
	uint start = (uint) heap + HEAP_S;
	heap_footer_t* foot = (heap_footer_t*) (start + size);
	foot->magic = KHEAP_MAGIC;
	foot->magic2 = KHEAP_MAGIC2;
	foot->size = size;
	
	size_t new_size = heap->size - HEAP_TOTAL - size;
	heap->size = size;

	heap = (heap_header_t*) ((uint) foot + sizeof(heap_footer_t));
	heap->size = new_size;
	heap->free = TRUE;
	heap->magic = KHEAP_MAGIC;
	heap->magic2 = KHEAP_MAGIC2;

	start = (uint) heap + HEAP_S;
	foot = (heap_footer_t*) (start + size);
	foot->magic = KHEAP_MAGIC;
	foot->magic2 = KHEAP_MAGIC2;
	if(foot->size != KHEAP_END)
		foot->size = new_size;
}

void free_internal(heap_header_t* heap, void* address){
	heap_header_t* head = (heap_header_t*) ((uint) address - HEAP_S);
	if(head == heap){
		WARN("can't collapse top of heap");
		return;
	}
	
	if((head->magic != KHEAP_MAGIC) || (head->magic2 != KHEAP_MAGIC2)){
		WARN("invalid header in heap");
		return;
	}
	
	heap_footer_t* foot = (heap_footer_t*) ((uint) head - sizeof(heap_footer_t*));
	if((foot->magic != KHEAP_MAGIC) || (foot->magic2 != KHEAP_MAGIC2)){
		WARN("invalid footer in heap");
		return;
	}

	if(foot->size = KHEAP_END)
		PANIC("impossible condition for heap");
	
	heap = (heap_header_t*) ((uint) foot - foot->size - HEAP_S);
	if((heap->magic != KHEAP_MAGIC) || (heap->magic2 != KHEAP_MAGIC2)){
		WARN("invalid parent in heap");
		return;
	}
	heap->size = heap->size + head->size + HEAP_TOTAL;

	foot = (heap_footer_t*) ((uint) foot + foot->size);
	if((foot->magic != KHEAP_MAGIC) || (foot->magic2 != KHEAP_MAGIC2))
		PANIC("fatal arithmetic error in free() call");

	foot->size = heap->size;
}

void* malloc_internal(heap_header_t* heap, size_t size){
	heap = find_sized_heap(heap, size);
	heap->free = FALSE;
	split_heap(heap, size);
	return (void*) ((uint) heap + HEAP_S);
}

void init_heap(heap_header_t* heap, size_t size){
	heap->magic = KHEAP_MAGIC;
	heap->magic2 = KHEAP_MAGIC2;
	heap->free = TRUE;
	heap->size = size - HEAP_TOTAL;

	heap_footer_t* foot = (heap_footer_t*) ((uint) heap + HEAP_S + heap->size);
	foot->magic = KHEAP_MAGIC;
	foot->magic2 = KHEAP_MAGIC2;
	foot->size = KHEAP_END;
}

void* kmalloc(uint size){
	if(kheap == NULL)
		return fmalloc(size);

	return malloc_internal(kheap, size);
}

void kfree(void* address){
	free_internal(kheap, address);
}
