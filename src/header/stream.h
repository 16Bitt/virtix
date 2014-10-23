#ifndef STREAM_H
#define STREAM_H

#include "common.h"
#include "kheap.h"

#define RAM_STREAM	0
#define DISK_STREAM	1
#define STREAM_READ	1
#define STREAM_WRITE	2

#define STREAM_RSZ_INC	64

extern heap_t* dheap;

typedef struct stream{
	unsigned int id;			//Stream ID
	
	unsigned int read_pos;		//reading position
	unsigned int write_pos;		//writing position
	unsigned int end_pos;		//Where the file actually ends

	char* buffer;		//Location of the actual data
	unsigned int	open;		//State of buffer
	unsigned int	type;		//Type of the buffer (ram|physical)
	unsigned int	size;		//Length of the buffer
	
	unsigned int	child;		//Whether or not this stream is original

	struct stream* next;
} stream_t;

//Initialize all of the streaming utillities
void	init_streams();

unsigned int	mk_stream();
void	rm_stream(unsigned int id);
void	stream_rsz(unsigned int id);
unsigned int	stream_rewind(unsigned int id);

unsigned int	clone_stream(unsigned int id, unsigned int mode);

unsigned int	fwrite_char(unsigned int stream_id, char val);
unsigned int	fread_char(unsigned int stream_id);

stream_t* find_stream(unsigned int id);

#endif
