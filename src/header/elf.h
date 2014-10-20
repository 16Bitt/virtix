#ifndef ELF_H
#define ELF_H

#include "common.h"
#include "proc.h"

typedef unsigned short	elf32_half;
typedef unsigned int	elf32_offs;
typedef unsigned int	elf32_addr;
typedef unsigned int	elf32_word;
typedef int		elf32_sword;

#define ELF_NIDENT	16

typedef struct {
	unsigned char	e_ident[ELF_NIDENT];
	elf32_half	e_type;
	elf32_half	e_machine;
	elf32_word	e_version;
	elf32_addr	e_entry;
	elf32_offs	e_phoff;
	elf32_offs	e_shoff;
	elf32_word	e_flags;
	elf32_half	e_ehsize;
	elf32_half	e_phentsize;
	elf32_half	e_phnum;
	elf32_half	e_shentsize;
	elf32_half	e_shnum;
	elf32_half	e_shstrndx;
} elf32_ehdr;

enum elf_ident {
	EI_MAG0		= 0, // 0x7F
	EI_MAG1		= 1, // 'E'
	EI_MAG2		= 2, // 'L'
	EI_MAG3		= 3, // 'F'
	EI_CLASS	= 4, // Architecture (32/64)
	EI_DATA		= 5, // Byte Order
	EI_VERSION	= 6, // ELF Version
	EI_OSABI	= 7, // OS Specific
	EI_ABIVERSION	= 8, // OS Specific
	EI_PAD		= 9  // Padding
};
		 
#define ELFMAG0		0x7F
#define ELFMAG1		'E'
#define ELFMAG2		'L'
#define ELFMAG3		'F'
	  
#define ELFDATA2LSB	(1)
#define ELFCLASS32	(1)

enum elf_type {
	ET_NONE		= 0, // Unkown Type
	ET_REL		= 1, // Relocatable File
	ET_EXEC		= 2  // Executable File
};

#define EM_386		(3)  // x86 Machine Type
#define EV_CURRENT	(1)  // ELF Current Version
#define ELF_RELOC_ERR	0xFFFFFFFF

typedef struct {
	elf32_word	sh_name;
	elf32_word	sh_type;
	elf32_word	sh_flags;
	elf32_addr	sh_addr;
	elf32_offs	sh_offset;
	elf32_word	sh_size;
	elf32_word	sh_link;
	elf32_word	sh_info;
	elf32_word	sh_addralign;
	elf32_word	sh_entsize;
} elf32_shdr;

 
 enum sht_types {
 	SHT_NULL	= 0,   // Null section
	SHT_PROGBITS	= 1,   // Program information
	SHT_SYMTAB	= 2,   // Symbol table
	SHT_STRTAB	= 3,   // String table
	SHT_RELA	= 4,   // Relocation (w/ addend)
	SHT_NOBITS	= 8,   // Not present in file
	SHT_REL		= 9,   // Relocation (no addend)
};
	 
enum sht_attributes {
	SHF_WRITE	= 0x01, // Writable section
	SHF_ALLOC	= 0x02  // Exists in memory
};

typedef struct {
	elf32_word		st_name;
	elf32_addr		st_value;
	elf32_word		st_size;
	unsigned char		st_info;
	unsigned char		st_other;
	elf32_half		st_shndx;
} elf32_sym;

#define ELF32_ST_BIND(INFO)	((INFO) >> 4)
#define ELF32_ST_TYPE(INFO)	((INFO) & 0x0F)

#define SHN_UNDEF       0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_HIRESERVE   0xffff

enum stt_bindings {
	STB_LOCAL		= 0, // Local scope
	STB_GLOBAL		= 1, // Global scope
	STB_WEAK		= 2  // Weak, (ie. __attribute__((weak)))
};
			 
enum stt_types {
 	STT_NOTYPE		= 0, // No type
	STT_OBJECT		= 1, // Variables, arrays, etc.
	STT_FUNC		= 2  // Methods or functions
};

typedef struct {
	elf32_addr		r_offset;
	elf32_word		r_info;
} elf32_rel;
		 
typedef struct {
 	elf32_addr		r_offset;
	elf32_word		r_info;
	elf32_sword		r_addend;
} elf32_rela;

#define ELF32_R_SYM(INFO)	((INFO) >> 8)
#define ELF32_R_TYPE(INFO)	((u8int)(INFO))
 
enum rtt_Types {
 	R_386_NONE		= 0, // No relocation
	R_386_32		= 1, // Symbol + Offset
	R_386_PC32		= 2  // Symbol + Offset - Section Offset
};

typedef struct {
	elf32_word		p_type;
	elf32_offs		p_offset;
	elf32_addr		p_vaddr;
	elf32_addr		p_paddr;
	elf32_word		p_filesz;
	elf32_word		p_memsz;
	elf32_word		p_flags;
	elf32_word		p_align;
} elf32_phdr;

#define DO_386_32(S, A)	((S) + (A))
#define DO_386_PC32(S, A, P)	((S) + (A) - (P))

bool 		elf_check_file(elf32_ehdr* header);
bool 		elf_check_supported(elf32_ehdr* header);
void* 		elf_load_rel(elf32_ehdr* header);
void* 		elf_load_file(void* file);
elf32_shdr*	elf_sheader(elf32_ehdr* header);
char*		elf32_str_table(elf32_ehdr* header);
char*		elf32_lookup_string(elf32_ehdr* header, int offset);
int		elf32_get_sym_val(elf32_ehdr* header, int table, unsigned int offset);
int		elf_load_stage1(elf32_ehdr* header);
int		elf_load_stage2(elf32_ehdr* header);
int		elf_do_reloc(elf32_ehdr* header, elf32_rel* rel, elf32_shdr* reltab);

#endif
