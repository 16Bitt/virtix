#ifndef DIRENT_H
#define DIRENT_H

#include "sys/ctypes.h"

__HEADER_START

#include "sys/stdlib.h"

typedef DIR uint;

struct dirent{
	ino_t	d_ino;
	char	dname[NAME_MAX]
};

int closedir(DIR* dir);
int opendir(DIR* dir);
struct dirent* readdir(DIR* dir);
int readdir_r(DIR* dir, struct dirent* dir, struct dirent** list);
void rewinddir(DIR* dir);
void seekdir(DIR* dir, uint to);
uint telldir(DIR* dir);

__HEADER_END

#endif
