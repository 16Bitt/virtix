#include "common.h"
#include "monitor.h"
#include "file.h"
#include "vfs.h"
#include "deepfat.h"

FILE kfopen(char* path, uint offset){
	fs_node_t* node = fs_path(df_root, path);
	
	if(node == NULL){
		vga_puts("WARN: kernel could not find file to open\n");
		return (FILE) -1;
	}
	
	return fd_create(node, offset);
}

void kfclose(FILE file){
	fd_delete(file);
}

uint kfread(FILE file, uint size, char* buffer){
	return fd_read(file, size, buffer);
}

uint kfwrite(FILE file, uint size, char* buffer){
	return fd_write(file, size, buffer);
}
