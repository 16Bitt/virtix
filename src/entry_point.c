//Kernel specific
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "multiboot.h"
#include "clock.h"
#include "fs.h"
#include "kheap.h"

//Untested
#include "virtix_page.h"
#include "flat.h"
#include "single.h"
#include "virtix_proc.h"

void* stack = NULL;

void waiting(registers_t regs){
	vga_puts("Waiting...\n");
	kthread_exit();
}

unsigned int stack_hold;

int main(struct multiboot* mboot_ptr, unsigned int esp){
	stack_hold = esp;
	vga_clear();
	
	vga_puts("main(): loaded Kernel\n");
	init_descriptor_tables();
	vga_puts("main(): initialized memory protection map\n");
	
	vga_puts("main(): making kheap\n");
	init_kheap();
	
	vga_puts("main(): attempting to initialize paging\n");
	virtix_page_init();

	vga_puts("main(): relocating modules\n");
	void* mod_loc = kmalloc_a(1024 * 10);
	memcpy(mod_loc, (void*) mboot_ptr->mods_addr, 1024 * 6);

	vga_puts("main(): initializing read-only ramdisk\n");
	mnt_initrd((unsigned int) mod_loc);
	
	vga_puts("main(): registering default handlers\n");
	register_default_handlers();

	vga_puts("main(): starting interrupts\n");
	sti();
	
	vga_puts("main(): loading a binary...\n");
	virtix_proc_t* proc = flat_load_bin(get_file_data("userland.x"));

	int x = 0;
	esp /= x;

	vga_puts("main(): attempting to hardload userspace\n");
	enter_userspace(proc);

	vga_puts("main(): reached end of execution, hanging the CPU\n");
	cli(); hlt();
}
