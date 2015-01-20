#ifndef __STAT_H
#define __STAT_H

#include "sys/cdefs.h"

__HEADER_START

struct stat{
	uint inode;
	uint length;
};

__HEADER_END

#endif
