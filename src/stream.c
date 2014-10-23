#include "common.h"
#include "stream.h"
#include "kheap.h"

#include "monitor.h"

stream_t* stream_root = NULL;

heap_t* dheap;

unsigned int stream_num = 0;

void init_streams(){
}

stream_t* find_stream(unsigned int id){
	stream_t* current = stream_root;
	while(current != NULL){
		if(current->id == id)
			return current;

		current = current->next;
	}

	return NULL;
}

stream_t* find_parent(stream_t* child){
	if(stream_root == child)
		return NULL;
	
	stream_t* current = stream_root;
	while(current != NULL){
		if(current->next == child)
			return current;

		current = current->next;
	}

	return (stream_t*) 0xFFFFFFFF;
}

unsigned int mk_stream(){
	stream_t* new_stream = (stream_t*) kmalloc( sizeof(stream_t));	
	
	if(stream_root == NULL)
		stream_root = new_stream;
	
	else{
		new_stream->next = stream_root->next;
		stream_root->next = new_stream;
	}

	//Create new id
	new_stream->id		= stream_num++;
	
	//Set all rw positions
	new_stream->read_pos	= 0;
	new_stream->write_pos	= 0;
	new_stream->end_pos	= 0;

	//Set file attributes
	new_stream->buffer	= (char*) kmalloc( STREAM_RSZ_INC);
	new_stream->open	= STREAM_READ;
	new_stream->type	= RAM_STREAM;
	new_stream->size	= STREAM_RSZ_INC;
	
	//This is the first instance pointing to this stream
	new_stream->child	= 0;

	return new_stream->id;
}

void rm_stream(unsigned int id){
	stream_t* open = find_stream(id);
	if(open == NULL)
		return;

	stream_t* parent = find_parent(open);
	if(parent == NULL){
		stream_root = NULL;
		return;
	}
	
	parent->next = open->next;
	if(open->child != 0)
		kfree( open->buffer);
	kfree( open);
}

void stream_rsz(unsigned int id){
	stream_t* open = find_stream(id);
	if(open == NULL)
		return;

	char* new_buf = kmalloc( open->size + STREAM_RSZ_INC);
	memcpy(new_buf, open->buffer, open->size);
	kfree( open->buffer);

	open->buffer = new_buf;
	open->size += STREAM_RSZ_INC;
}

unsigned int fwrite_char(unsigned int id, char val){
	stream_t* open = find_stream(id);
	
	if(open == NULL)
		return 0xFFFFFFFF;
	
	open->buffer[open->write_pos++] = val & 0xFF;
	
	if(open->write_pos > open->end_pos)
		open->end_pos = open->write_pos;

	if(open->end_pos >= open->size)
		stream_rsz(id);

	return 0;
}

unsigned int fread_char(unsigned int id){
	stream_t* open = find_stream(id);
	
	if(open == NULL)
		return 0xFFFFFFFF;
	
	//vga_puts("Survived?");
	if(open->read_pos > open->end_pos)
		return 0xFFFFFFFF;
	
	return open->buffer[open->read_pos++];
}

unsigned int stream_rewind(unsigned int id){
	stream_t* open = find_stream(id);

	if(open == NULL)
		return 0xFFFFFFFF;
	
	//Simply change read position
	open->read_pos = 0;

	return 0;
}

unsigned int clone_stream(unsigned int id, unsigned int mode){
	stream_t* open = find_stream(id);
	if(open == NULL)
		return 0xFFFFFFFF;

	stream_t* new_stream = (stream_t*) kmalloc( sizeof(stream_t));
	memcpy(new_stream, open, sizeof(stream_t));
	
	new_stream->id = stream_num++;

	if(stream_root == NULL)
		stream_root = new_stream;
	
	else{
		new_stream->next = stream_root->next;
		stream_root->next = new_stream;
	}

	switch(mode){
		//Use the stream as-is
		case 0:
			return new_stream->id;
		
		//Use the stream from the beginning
		case 1:
			new_stream->write_pos = 0;
			new_stream->read_pos = 0;
			return new_stream->id;

		//Invalid mode, bail out
		default:
			rm_stream(new_stream->id);
			return 0xFFFFFFFF;
	}

	return 0xFFFFFFFF;
}
