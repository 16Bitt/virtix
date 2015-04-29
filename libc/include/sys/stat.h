#ifndef __STAT_H
#define __STAT_H

#include "sys/cdefs.h"

__HEADER_START

#include "sys/types.h"

#define S_IFMT	1
#define S_IFDIR	2
#define S_IFREG	4
#define S_ISUID	8
#define S_ISGID	16
#define S_ISVTX	32
#define S_IXUSR	64
#define S_IXOTH	128
#define S_IXGRP	256

typedef struct stat{
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
} stat_t;

#define S_ISDIR(m) ((m) & S_IFDIR)
#define S_ISREG(m) ((m) & S_IFREG)

int stat(const char* path, struct stat* st);
int fstat(int fd, struct stat* st);

__HEADER_END

#endif
