#include "common.h"
#include "kheap.h"
#include "monitor.h"

unsigned int placement_address;

void init_kheap(){
	end = (unsigned int) &end;
	placement_address = end;
}

void* kmalloc(unsigned int size){
	ASSERT(placement_address + size < MEM_END);
	unsigned int hold = placement_address;
	memset((void*) hold, 0, size);
	placement_address += size;
	return (void*) hold;
}

void kfree(void* ptr){
	WARN("fake stub called")
	return;
}

void* kmalloc_a(unsigned int size){
	ASSERT(((placement_address & 0xFFFFF000) + 0x1000) + size < MEM_END);
	placement_address &= 0xFFFFF000;
	placement_address += 0x1000;

	unsigned int hold = placement_address;
	placement_address += size;
	
	return (void*) hold;
}
