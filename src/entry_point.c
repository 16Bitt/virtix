//Kernel specific
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "kheap.h"
#include "multiboot.h"
#include "clock.h"

//Untested
#include "paging.h"

heap_t* kheap;

int main(struct multiboot* mboot_ptr){
	vga_clear();
	vga_puts("main(): loaded Kernel\n");
	init_descriptor_tables();
	vga_puts("main(): initialized memory protection map\n");
	
	vga_puts("main(): attempting to initialize paging\n");
	init_paging();
	
	vga_puts("main(): making 2 bheaps\n");
	init_kheaps(4);
	kheap = kbmalloc("KERNEL");

	vga_puts("main(): initializing read-only ramdisk\n");
	mnt_initrd(mboot_ptr->mods_addr);

	vga_puts("main(): starting interrupts\n");
	sti();

	*((int*) 0xFFFFFFFF) = 0;

	vga_puts("main(): reached end of execution, hanging the CPU");
	for(;;);
}
