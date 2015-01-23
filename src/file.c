#include "common.h"
#include "monitor.h"
#include "file.h"
#include "vfs.h"
#include "deepfat.h"
#include "fd.h"

FILE kfopen(char* path, uint offset){
	fs_node_t* node = fs_path(df_root, path);
	vga_puts_hex((uint) node);
	NOTIFY(path)
	
	if(node == NULL){
		node = vfs_touch(df_root, path);
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

uint kfstat(FILE file, struct stat* buffer){
	return fd_stat(file, buffer);
}

uint kstat(char* path, struct stat* buffer){
	fs_node_t* node = fs_path(df_root, path);

	if(node == NULL)
		return (uint) -1;

	buffer->inode = node->inode;
	buffer->length = buffer->length;

	return 0;
}

uint klseek(FILE file, uint offset, uint direction){
	struct stat status;
	uint err = fd_stat(file, &status);

	if(err != 0)
		return (uint) -1;

	switch(direction){
		case SEEK_SET:
			fd_seek(file, offset);
			break;
		case SEEK_CUR:
			fd_seek(file, fd_offset(file) + offset);
			break;
		case SEEK_END:
			fd_seek(file, status.length + offset);
			break;

		default:
			return (uint) -1;
	}

	return 0;
}
