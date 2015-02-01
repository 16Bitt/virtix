#ifndef DEV_H
#define DEV_H

#include "common.h"
#include "vfs.h"

struct dev;

typedef uint (*read_block_t)	(struct dev*, uint, char*);
typedef uint (*write_block_t)	(struct dev*, uint, char*);

typedef struct dev{
	char	name[128];
	size_t	block_size;
	read_block_t	read;
	write_block_t	write;

	uchar bus;
	uchar drive;
} dev_t;

void mkdev(char* name, read_type_t read, write_type_t write);

uint dev_read(dev_t* dev, uint offset, char* buffer);
uint dev_write(dev_t* dev, uint offset, char* buffer);

#endif
