#ifndef __STAT_H
#define __STAT_H

#include "sys/cdefs.h"

__HEADER_START

#include "sys/types.h"

struct stat{
	dev_t	st_dev;
	ino_t	st_ino;
	mode_t	st_mode;
	nlink_t	st_nlink;
	uid_t	st_uid;
	gid_t	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	time_t	st_mtime;
	time_t	st_ctime;
	blksize_t	st_blksize;
	blkcnt_t	st_blocks;
};

__HEADER_END

#endif
