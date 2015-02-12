#include "common.h"
#include "file.h"
#include "exec.h"
#include "virtix_proc.h"
#include "elf.h"
#include "monitor.h"

uint kexec(char* path){
	FILE f = kfopen(path, 0);
	if(f == (uint) -1)
		return (uint) -1;

	struct stat* st = (struct stat*) kmalloc(sizeof(struct stat));
	kfstat(f, st);
	size_t size = st->length;
	kfree(st);
	char* buffer = (char*) kmalloc(size);
	kfread(f, size, buffer);
	virtix_proc_t* proc = elf_load((void*) buffer);
	kfree(buffer);
	if(proc == NULL)
		return (uint) -1;
	
	enter_userspace(proc);
}
