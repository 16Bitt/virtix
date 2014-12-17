#ifndef FILE_H
#define FILE_H

typedef uint FILE;
typedef uint DIR;

FILE kfopen(char* path, uint offset);
void kfclose(FILE file);
uint kfwrite(FILE file, uint size, char* buffer);
uint kfread(FILE file, uint size, char* buffer);

#endif
