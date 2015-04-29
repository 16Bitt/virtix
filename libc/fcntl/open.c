#include "fcntl.h"
#include "syscall.h"
#include "stdarg.h"
#include "unistd.h"
#include "sys/stat.h"
#include "errno.h"

int open(const char* path, int flags, ...){
	struct stat st;
	
	//Check that the file doesn't exist, if requested
	if(flags | O_EXCL){
		int err = stat(path, &st);
		if(!err)
			return -1;
	}

	int fd = syscall_2arg(SYS_OPEN, (uint) path, 0);
	
	if(fd < 0){
		errno = ENOENT;
		return -1;
	}

	fstat(fd, &st);

	if(flags | O_APPEND)
		lseek(fd, st.st_size, SEEK_SET);

	return fd;
}
