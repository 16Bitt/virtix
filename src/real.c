#include "common.h"
#include "real.h"
#include "monitor.h"
#include "clock.h"
#include "file.h"

void run_stub16(char* file){
	FILE f = kfopen(file, 0);
	struct stat* st = (struct stat*) kmalloc(sizeof(struct stat));
	kfstat(f, st);
	if(st->length == 0){
		kfclose(f);
		WARN("16-bit stub load failed: empty file");
		return;
	}
	
	kfread(f, st->length, (char*) 0x7C00);

	WARN("Entering real mode in 3 seconds");
	sleep(3);
	WARN("cross your fingers")
	cli();
	hlt();
	asm volatile ("jmp 0x7C00");
}
