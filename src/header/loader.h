#ifndef LOADER_H
#define LOADER_H

#include "common.h"
#include "elf.h"
#include "proc.h"

void* bin_get_init(elf32_ehdr* header);
void hook_proc(elf32_ehdr* header, PROC* proc);

#endif
