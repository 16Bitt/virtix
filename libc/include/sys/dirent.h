#ifndef __DIRENT_H
#define __DIRENT_H

#include "sys/cdefs.h"

__HEADER_START

struct dirent{
	char name[128];
	uint ino;
};

__HEADER_END

#endif
