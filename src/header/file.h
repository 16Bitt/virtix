#ifndef FILE_H
#define FILE_H

#include "common.h"
#include "vfs.h"

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

typedef uint FILE;
typedef uint DIR;

FILE kfopen(char* path, uint offset);
void kfclose(FILE file);
uint kfwrite(FILE file, uint size, char* buffer);
uint kfread(FILE file, uint size, char* buffer);
uint kfstat(FILE file, struct stat* buffer);
uint kstat(char* path, struct stat* buffer);
uint klseek(FILE file, uint offset, uint direction);
uint kfreadchar(FILE file, char* c);
uint kfwritechar(FILE file, char* c);

#endif
