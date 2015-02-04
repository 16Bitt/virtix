#ifndef DEV_H
#define DEV_H

#include "common.h"

struct dev;
typedef struct fs_node fs_node_t;

typedef uint (*read_block_t)	(fs_node_t*, uint, char*);
typedef uint (*write_block_t)	(fs_node_t*, uint, char*);

typedef struct dev{
	char	name[128];
	size_t	block_size;
	read_block_t	read;
	write_block_t	write;

	uchar bus;
	uchar drive;
} dev_t;

//void mkdev(char* name, read_type_t read, write_type_t write);

uint dev_read(fs_node_t* dev, uint offset, char* buffer);
uint dev_write(fs_node_t* dev, uint offset, char* buffer);

#endif
