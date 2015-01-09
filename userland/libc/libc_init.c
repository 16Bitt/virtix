#include "sys/types.h"
#include "sys/virtix.h"

extern int main(char** argv, int argc);

void libc_init(){
	char** args	= virtix_query_args();
	size_t arglen	= virtix_query_arglen();
	int ret_code	= main(args, arglen);
	virtix_exit(ret_code);
}

void _exit(int ret_code){
	virtix_ret(ret_code);
}
