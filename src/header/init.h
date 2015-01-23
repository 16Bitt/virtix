#ifndef INIT_H
#define INIT_H

#include "common.h"

#define DEVLOAD(x) extern void x(); x();

void driver_init();

#endif
