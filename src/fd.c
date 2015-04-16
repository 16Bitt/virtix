#include "common.h"
#include "monitor.h"
#include "vfs.h"
#include "fd.h"
#include "deepfat.h"
#include "kheap.h"
#include "str-util.h"

fd_t fd_list[MAX_FD];

void init_fd(){
	memset(fd_list, 0, sizeof(fd_t) * MAX_FD);
}

fs_node_t* fd_lookup(uint fd){
	if(fd >= MAX_FD)
		return NULL;

	return fd_list[fd].node;
}

uint calc_blk_offset(uint bs, uint offs){
	uint index = 0;
	
	if(bs == 1)
		return offs;

	while(bs < offs){
		index++;
		offs--;
	}

	return index;
}

uint calc_buf_offset(uint bs, uint offs){
	return bs * calc_blk_offset(bs, offs) - offs;
}

uint fd_create(fs_node_t* node, uint offset){
	int i;

	for(i = 0; i < MAX_FD; i++){
		if(fd_list[i].present == 0){
			fd_list[i].node		= node;
			strmov(fd_list[i].name, node->name);
			fd_list[i].block_size	= node->dev->block_size;
			fd_list[i].buffer	= (char*) kmalloc(node->dev->block_size);
			fd_list[i].present	= 1;
			
			if(node->dev->block_size != 0)
				read_fs(node, calc_blk_offset(node->dev->block_size, offset), fd_list[i].buffer);
			
			fd_list[i].offset	= calc_buf_offset(node->dev->block_size, offset);
			fd_list[i].fs_size	= node->length;
			
			//vga_fmt("FD for %s created with %d byte blocks, %d byte length\n", node->name, fd_list[i].block_size, node->length);

			return i;
		}
	}

	return (uint) -1;
}

void fd_delete(uint fd){
	if(fd >= MAX_FD)
		return;
	
	kfree(fd_list[fd].buffer);
	memset(&fd_list[fd], 0, sizeof(fd_t));
}

void fd_seek(uint fd, uint offset){
	if(fd >= MAX_FD)
		return;
	
	fd_flush(fd);

	fd_list[fd].block = calc_blk_offset(fd_list[fd].node->dev->block_size, offset);
	fd_list[fd].offset = calc_buf_offset(fd_list[fd].node->dev->block_size, offset);
}

struct dirent* fd_readdir(uint fd){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;

	return readdir_fs(node, fd_list[fd].offset++);
}

uint fd_read(uint fd, uint size, char* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;

	int i, status;
	char read;
	
	if(node->dev->block_size == 0){		//Instant update device (stdout etc.)
		for(i = 0; i < size; i++)
			status = node->read_blk(node, 0, &buffer[i]);
	}
	
	else
		for(i = 0; i < size; i++){
			status = fd_readch(fd, &read);
			buffer[i] = read;
		}
	
	return status;
}

uint fd_write(uint fd, uint size, char* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return NULL;
	
	int i, status;

	if(node->dev->block_size == 0){		//Instant update device (stdout etc.)
		for(i = 0; i < size; i++)
			status = node->write_blk(node, 0, &buffer[i]);
	}
	
	else
		for(i = 0; i < size; i++)
			status = fd_writech(fd, &buffer[i]);

	return status;

}

size_t calc_total_size(uint fd){
	return (fd_list[fd].block * fd_list[fd].block_size) + fd_list[fd].offset;
}

uint fd_readch(uint fd, char* c){
	if(fd >= MAX_FD){
		WARN("bad file descriptor")
		return (uint) -1;
	}

	fd_t* desc = &fd_list[fd];
	*c = desc->buffer[desc->offset++];

	if(calc_blk_offset(desc->block_size, desc->fs_size) == desc->block)
		if(calc_buf_offset(desc->block_size, desc->fs_size) <= desc->offset){
			WARN("hit buffer end")
			return (uint) -1;
		}

	if(desc->offset >= desc->block_size){
		desc->offset = 0;
		desc->block++;
		desc->node->read_blk(desc->node, desc->block, desc->buffer);
	}
	
	return 0;
}

uint fd_writech(uint fd, char* c){
	if(fd >= MAX_FD)
		return (uint) -1;

	fd_t* desc = &fd_list[fd];
	desc->buffer[desc->offset++] = *c;
	
	if(desc->offset >= desc->block_size){
		desc->offset = 0;
		desc->node->write_blk(desc->node, desc->block, desc->buffer);
		desc->block++;
	}

	return 0;
}

uint fd_flush(uint fd){
	if(fd >= MAX_FD)
		return (uint) -1;

	fd_t* desc = &fd_list[fd];
	desc->node->length = calc_total_size(fd);
	
	if(desc->block_size == 0)
		return 0;

	return desc->node->write_blk(desc->node, desc->block, desc->buffer);
}

uint fd_stat(uint fd, struct stat* buffer){
	fs_node_t* node = fd_lookup(fd);
	if(node == NULL)
		return (uint) -1;

	buffer->length = node->length;
	buffer->inode = node->inode;

	return 0;
}

uint fd_offset(uint fd){
	if(fd >= MAX_FD)
		return 0;

	return fd_list[fd].offset;
}
