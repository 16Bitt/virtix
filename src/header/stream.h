#ifndef STREAM_H
#define STREAM_H

#include "common.h"
#include "vproc.h"
#include "kheap.h"

#define RAM_STREAM	0
#define DISK_STREAM	1
#define STREAM_READ	1
#define STREAM_WRITE	2

#define STREAM_RSZ_INC	64

extern heap_t* dheap;

typedef struct stream{
	var id;			//Stream ID
	
	var read_pos;		//reading position
	var write_pos;		//writing position
	var end_pos;		//Where the file actually ends

	char* buffer;		//Location of the actual data
	var	open;		//State of buffer
	var	type;		//Type of the buffer (ram|physical)
	var	size;		//Length of the buffer
	
	var	child;		//Whether or not this stream is original

	struct stream* next;
} stream_t;

//Initialize all of the streaming utillities
void	init_streams();

var	mk_stream();
void	rm_stream(var id);
void	stream_rsz(var id);
var	stream_rewind(var id);

var	clone_stream(var id, var mode);

var	fwrite_char(var stream_id, char val);
var	fread_char(var stream_id);

stream_t* find_stream(var id);

#endif
