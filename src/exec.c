#include "common.h"
#include "file.h"
#include "exec.h"
#include "virtix_proc.h"
#include "elf.h"
#include "monitor.h"
#include "kheap.h"

uint kexec(char* path){
	FILE f = kfopen(path, 0);
	if(f == (FILE) -1)
		return (uint) -1;

	struct stat* st = (struct stat*) kmalloc(sizeof(struct stat));
	kfstat(f, st);
	size_t size = st->st_size;
	kfree(st);
	char* buffer = (char*) kmalloc(size);
	kfread(f, size, buffer);
	virtix_proc_t* proc = elf_load((void*) buffer);
	kfree(buffer);
	if(proc == NULL)
		return (uint) -1;
	
	proc->stdin = kfopen("/dev/stdin", 0);
	proc->stdout = kfopen("/dev/stdout", 0);
	proc->stderr = kfopen("/dev/stderr", 0);
	
	NOTIFY(path);
	init_procs(proc);
}

uint kexec_pipes(char* path, char* stdin, char* stdout, char* stderr){
	FILE f = kfopen(path, 0);
	if(f == (FILE) -1)
		return (uint) -1;

	struct stat* st = (struct stat*) kmalloc(sizeof(struct stat));
	kfstat(f, st);
	size_t size = st->st_size;
	kfree(st);
	char* buffer = (char*) kmalloc(size);
	kfread(f, size, buffer);
	virtix_proc_t* proc = elf_load((void*) buffer);
	kfree(buffer);
	if(proc == NULL)
		return (uint) -1;
	
	proc->stdin = kfopen("/dev/stdin", 0);
	proc->stdout = kfopen("/dev/stderr", 0);
	proc->stderr = kfopen("/dev/stderr", 0);
	
	init_procs(proc);
}

uint uexec(char* path){
	//NOTIFY("trapped exec() call")
	susp_proc(current_proc->pid);
	
	FILE f = kfopen(path, 0);
	if(f == (uint) -1){
		wake_proc(current_proc->pid);
		return (uint) -1;
	}

	struct stat* st = (struct stat*) kmalloc(sizeof(struct stat));
	kfstat(f, st);
	size_t size = st->st_size;
	kfree(st);
	char* buffer = (char*) kmalloc(size);
	kfread(f, size, buffer);
	virtix_proc_t* proc = elf_load((void*) buffer);
	kfree(buffer);
	if(proc == NULL)
		return (uint) -1;
	
	proc->stdin = kfopen("/dev/stdin", 0);
	proc->stdout = kfopen("/dev/stdout", 0);
	proc->stderr = kfopen("/dev/stderr", 0);
	proc->parent = current_proc;
	proc->gid = current_proc->pid;

	spawn_proc(proc);
	return 0;
}
