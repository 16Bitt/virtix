//Kernel specific
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "multiboot.h"
#include "fs.h"
#include "kheap.h"
#include "virtix_page.h"

//Untested
#include "virtix_proc.h"
#include "elf.h"
#include "fat.h"
#include "ata.h"
#include "deepfat.h"
#include "file.h"

void* stack = NULL;

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
	
	vga_puts("main(): registering userspace handler\n");
	init_userspace();

	vga_puts("main(): starting interrupts\n");
	sti();
	
	/*
	vga_puts("main(): loading a binary...\n");
	virtix_proc_t* proc = elf_load(get_file_data("hello.x"));

	vga_puts("main(): entering usermode\n");
	enter_userspace(proc);
	*/

	vga_puts("main(): starting ATA driver\n");
	init_ata();
	
	vga_puts("main(): starting FAT driver\n");
	init_fat();
	disp_fat_dir();

	vga_puts("main(): starting deepFAT driver\n");
	init_deepfat();
	
	vga_puts("main(): starting file descriptor interface\n");
	init_fd();

	vga_puts("main(): testing file interface\n");
	FILE f = kfopen("header/test.txt", 0);
	char* str = "Hello, World!\n";
	kfwrite(f, strlen(str), str);
	kfclose(f);

	f = kfopen("header/test.txt", 0);
	kfread(f, strlen(str), str);
	vga_puts(str);

	vga_puts("main(): syncing FAT\n");
	fat_sync();

	vga_puts("main(): reached end of execution, hanging the CPU\n");
	cli(); hlt();
}
