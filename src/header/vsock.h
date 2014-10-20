#ifndef VSOCK_H
#define VSOCK_H

//For 'var' datatype
#include "vproc.h"

//Number of sockets to use
#define VSOCK_SIZE 16

typedef struct{
	//Required priveledge, smaller is higher priveledge
	var priveledge;
	//Pid of owner, 0 if public
	var pid;

	//If not zero, socket is locked
	var locked;
	//If not zero, socket is ready to read and return
	var ready;

	//Location to write
	var input;
	//Location to read
	var output;
} vsock_t;

//Table of all vsocks
extern vsock_t** vsock_list;

//Initialize vsock table
void init_vsocks();
//Make a socket
var	mk_vsock(var priveledge, var pid);
//Remove a socket
var	rm_vsock(var sock_id);

#endif
