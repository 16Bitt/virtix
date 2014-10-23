#include "common.h"
#include "kheap.h"
#include "monitor.h"

unsigned int placement_address;

void init_kheap(){
	placement_address = end;
}

void* kmalloc(unsigned int size){
	ASSERT(placement_address + size < MEM_END);
	unsigned int hold = placement_address;
	placement_address += size;
	return (void*) hold;
}

void kfree(void* ptr){
	vga_puts("WARN: leak at ");
	vga_puts_hex(ptr);
	vga_puts(" because of fake kfree() call\n");
	return;
}

void* kmalloc_a(unsigned int size){
	ASSERT(((placement_address & 0xFFFFF000) + 1024) + size < MEM_END);
	placement_address &= 0xFFFFF000;
	placement_address += 1024;
	
	unsigned int hold = placement_address;
	placement_address += size;

	return (void*) hold;
}
