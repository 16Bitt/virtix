#ifndef DIRENT_H
#define DIRENT_H

#include "sys/ctypes.h"

__HEADER_START

#include "stdlib.h"

#ifndef NAME_MAX
#define NAME_MAX 256
#endif

typedef uint DIR;

struct dirent{
	ino_t	d_ino;
	char	d_name[NAME_MAX];
};

int closedir(DIR* dir);
int opendir(DIR* dir);
struct dirent* readdir(DIR* dir);
int readdir_r(DIR* dir, struct dirent* entry, struct dirent** list);
void rewinddir(DIR* dir);
void seekdir(DIR* dir, uint to);
uint telldir(DIR* dir);

__HEADER_END

#endif
