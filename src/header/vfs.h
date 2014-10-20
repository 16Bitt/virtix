#ifndef VFS_H
#define VFS_H

#include "common.h"
#include "stream.h"

typedef struct vfs_node{
	char*			name;	//Name of node
	void* 			buffer;	//Address of data
	unsigned int		size;	//size of data
	struct vfs_node*	next;	//Pointer to next node in linked list
	struct vfs_node*	last;	//Parent
} vfs_node_t;

//Constants for more readable code
#define FILE_EXIST 1
#define FILE_NO_EXIST 0

#define FILE_BOT 0
#define FILE_TOP 1

//Initializes all state required for the vfs
void		init_vfs();
//Creates a new file from RAM
void		vfs_ramload(char* name, void* address, unsigned int size);

//Associate a stream with a name
unsigned int	vfs_assoc(char* name, stream_t* stream);		
//Remove (delete) a file
unsigned int	vfs_rm(char* name);
//Copy a file into another
unsigned int	vfs_cp(char* orig, char* new_file);
//Renames a file
unsigned int	vfs_reassoc(char* name, char* new_name);
//Check if a file exists
unsigned int	vfs_isfile(char* name);

//Copies file into a stream and returns the stream
var		vfs_open(char* name, unsigned int mode);

//Writes all of the file names in the VFS to the desired stream
void		vfs_list(var stream_id);

#endif
