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

uint kexec_pipes(char* path, char* stdin, char* stdout, char* stderr){
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
	
	//proc->stdin = kfopen(stdout, 0);
	proc->stdout = kfopen("/dev/stdout", 0);
	proc->stderr = kfopen("/dev/stderr", 0);
	
	enter_userspace(proc);
}
