#ifndef DEV_H
#define DEV_H

#include "common.h"
#include "vfs.h"

void mkdev(char* name, read_type_t read, write_type_t write);

#endif
