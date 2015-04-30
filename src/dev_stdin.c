#include "common.h"
#include "dev.h"
#include "vfs.h"
#include "monitor.h"
#include "deepfat.h"
#include "isr.h"
#include "keyboard.h"

extern uchar keyboard_getc();

uint dev_stdin_read(fs_node_t* node, uint offset, uchar* buffer){
	sti();	//Reenable-- this is a safe place to allow interrupts in ring 0 

	wait_for_update:
		*buffer = keyboard_getc();

	if(*buffer == 0){
		hlt();
		goto wait_for_update;
	}
	
	cli();	//Disable them again just in case
	return 0;
}

void dev_stdin(){
	mkdev("stdin", dev_stdin_read, NULL);
	fs_node_t* node = fs_path(df_root, "/dev/stdin");
	node->dev->block_size = 0;
	node->length = 0xFFFFFFFF;
}
