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
#include "init.h"

void* stack = NULL;

unsigned int stack_hold;

int main(struct multiboot* mboot_ptr, unsigned int esp){
	stack_hold = esp;
	vga_reset();
	vga_clear();
	
	NOTIFY("loaded Kernel")
	init_descriptor_tables();
	NOTIFY("initialized memory protection map")
	
	NOTIFY("making kheap")
	init_kheap();
	
	NOTIFY("attempting to initialize paging")
	virtix_page_init();

	/*
	vga_puts("main(): relocating modules\n");
	void* mod_loc = kmalloc_a(1024 * 10);
	memcpy(mod_loc, (void*) mboot_ptr->mods_addr, 1024 * 6);

	vga_puts("main(): initializing read-only ramdisk\n");
	mnt_initrd((unsigned int) mod_loc);
	*/

	NOTIFY("registering default handlers")
	register_default_handlers();
	
	NOTIFY("registering userspace handler")
	init_userspace();

	NOTIFY("starting interrupts")
	sti();
	
	/*
	vga_puts("main(): loading a binary...\n");
	virtix_proc_t* proc = elf_load(get_file_data("hello.x"));

	vga_puts("main(): entering usermode\n");
	enter_userspace(proc);
	*/

	NOTIFY("starting ATA driver")
	init_ata();
	
	NOTIFY("starting FAT driver")
	init_fat();

	NOTIFY("starting deepFAT driver")
	init_deepfat();
	
	NOTIFY("starting file descriptor interface")
	init_fd();

	NOTIFY("starting VFS drivers");
	driver_init();
	
	FILE f = kfopen("/dev/stdout", 0);
	char* msg = "Test write to /dev/stdout\n";
	kfwrite(f, strlen(msg), msg);

	vfs_ls("/dev");

	NOTIFY("syncing FAT")
	fat_sync();
	
	WARN("reached end of execution, hanging the CPU")
	cli(); hlt();
}
