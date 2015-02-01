#ifndef STREAM_H
#define STREAM_H

#include "common.h"

typedef struct {
	uint block;
	uint offset;
	size_t size;
	char* data;
} stream_t;

stream_t* mk_stream(size_t size);
void rm_stream;

#endif
