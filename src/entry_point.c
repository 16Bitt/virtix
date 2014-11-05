//Kernel specific
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "multiboot.h"
#include "clock.h"
#include "fs.h"

//Untested
#include "kheap.h"
#include "paging.h"
#include "elf.h"
#include "virtix_proc.h"
#include "flat.h"

void* stack = NULL;

void waiting(registers_t regs){
	while(1)
	vga_puts("Waiting...");
}

unsigned int stack_hold;

int main(struct multiboot* mboot_ptr, unsigned int esp){
	stack_hold = esp;
	vga_clear();
	vga_puts("main(): loaded Kernel\n");
	init_descriptor_tables();
	vga_puts("main(): initialized memory protection map\n");
	
	vga_puts("main(): attempting to initialize paging\n");
	init_paging();
	
	vga_puts("main(): making kheap\n");
	init_kheap();
	
	vga_puts("main(): initializing read-only ramdisk\n");
	mnt_initrd(mboot_ptr->mods_addr);

	vga_puts("main(): starting interrupts\n");
	sti();
	
	vga_puts("main(): starting scheduler\n");
	init_procs(&waiting);

	vga_puts("main(): attempting to load thread\n");
	virtix_proc_t* proc = mk_empty_proc();

	proc = flat_load_bin(get_file_data("userland.x"));
	spawn_proc(proc);

	vga_puts("main(): reached end of execution, hanging the CPU");
	for(;;);
}
