#include "common.h"
#include "elf.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "kheap.h"
#include "virtix_page.h"

unsigned char validate_elf_data[] = { ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3 };

int validate_elf(void* data){
	elf32_ehdr* header = (elf32_ehdr*) data;

	int i;
	for(i = 0; i < 4; i++)
		if(header->e_ident[i] != validate_elf_data[i])
			return 0;
	
	return 1;
}

virtix_proc_t* elf_load(void* elf_data){
	if(validate_elf(elf_data) == 0){
		vga_puts("elf_load(): validation failed\n");
		return (virtix_proc_t*) NULL;
	}

	elf32_ehdr* header = (elf32_ehdr*) elf_data;

	if(header->e_type != 2){
		vga_puts("elf_load(): wrong executable type\n");
		return (virtix_proc_t*) NULL;
	}

	virtix_proc_t* proc = mk_empty_proc();
	proc->name = "ELF_PROGRAM";
	proc->registers.eip = header->e_entry;

	switch_vpage_dir(proc->cr3);
	uint stk = (uint) kmalloc_a(PAGE_S);
	proc->registers.esp = 0x4000000 - (PAGE_S / 2);
	proc->registers.ebp = proc->registers.esp;
	proc->registers.useresp = proc->registers.esp;
	vpage_map_user(proc->cr3, stk, 0x4000000 - PAGE_S);

	elf32_phdr* phdr = (elf32_phdr*) (((unsigned int) elf_data) + header->e_phoff);

	int i;
	for(i = 0; i < header->e_phnum; i++, phdr++){
		switch(phdr->p_type){
			case 0:
				break;
			case 1:
				vga_puts("Allocating space for ELF binary section...\n");
				unsigned int loc = (unsigned int) kmalloc_a(PAGE_S);
				vpage_map_user(proc->cr3, loc, phdr->p_vaddr);
				memcpy((void*) phdr->p_vaddr, ((void*) ((unsigned int) elf_data) + phdr->p_offset), phdr->p_filesz);
				if(phdr->p_filesz > PAGE_S)
					PANIC("ELF binary section too large");
				break;
			default:
				vga_set_fg(RED);
				vga_puts("type=");
				vga_puts_hex(phdr->p_type);
				vga_puts("\n");
				WARN("unknown header type");
		}
	}

	switch_vpage_dir(root_vpage_dir);
	return proc;
}
