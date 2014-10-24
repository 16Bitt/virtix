#include "common.h"
#include "elf.h"
#include "monitor.h"
#include "virtix_proc.h"
#include "kheap.h"
#include "paging.h"

int validate_elf(void* data){
	elf32_ehdr* header = (elf32_ehdr*) data;
	unsigned char* validate = { ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3 };

	int i;
	for(i = 0; i < 4; i++)
		if(header->e_ident[i] != validate[i])
			return 0;
	
	return 1;
}

virtix_proc_t* elf_load(void* elf_data){
	if(validate_elf(elf_data) == 0)
		return (virtix_proc_t*) NULL;

	elf32_ehdr* header = (elf32_ehdr*) elf_data;

	if(header->e_type != 2)
		return (virtix_proc_t*) NULL;

	unsigned int* cr3 = mk_page();
	elf32_phdr* phdr = (elf32_phdr*) (((unsigned int) elf_data) + header->e_phoff);

	int i;
	for(i = 0; i < header->e_phnum; i++, phdr++){
		switch(phdr->p_type){
			case 0:
				break;
			case 1:
				vga_puts("Making elf space\n");
				unsigned int loc = (unsigned int) kmalloc_a(PAGE_S);
				mmap_page(cr3, phdr->p_vaddr, loc);
				memcpy((void*)(loc + phdr->p_vaddr), (void*) (((unsigned int) elf_data) + phdr->p_offset), phdr->p_filesz);
				break;
			default:
				PANIC("unknown header in elf parser");
			return 0;
		}
	}

	virtix_proc_t* proc;// = mk_empty_proc();
	proc->name = "ELF_PROGRAM";
	proc->cr3 = cr3;
}
