#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "common.h"

#define MULTIBOOT_FLAG_MEM 	1
#define MULTIBOOT_FLAG_DEVICE	2
#define MULTIBOOT_FLAG_CMDLINE	4
#define MULTIBOOT_FLAG_MODS	8
#define MULTIBOOT_FLAG_AOUT	0x10
#define MULTIBOOT_FLAG_ELF	0x20
#define MULTIBOOT_FLAG_MMAP	0x40
#define MULTIBOOT_FLAG_CONFIG	0x80
#define MULTIBOOT_FLAG_LOADER	0x100
#define MULTIBOOT_FLAG_APM	0x200
#define MULTIBOOT_FLAG_VBE	0x400

struct multiboot{
	unsigned int 	flags,
			mem_lower,
			mem_upper,
			boot_device,
			cmdline,
			mods_count,
			mods_addr,
			num,
			size,
			addr,
			shndx,
			mmap_length,
			mmap_addr,
			drives_length,
			drives_addr,
			config_table,
			boot_loader_name,
			apm_table,
			vbe_control_info,
			vbe_mode_info,
			vbe_mode,
			vbe_interface_seg,
			vbe_interface_off,
			vbe_interface_len;
}__attribute__((packed));

typedef struct multiboot_header multiboot_header_t;

#endif
