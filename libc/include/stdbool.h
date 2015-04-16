#ifndef STDBOOL_H
#define STDBOOL_H

#include "sys/cdefs.h"

__HEADER_START

typedef _Bool int;

#define bool _Bool
#define true 1
#define false 0

#define __bool_true_false_are_defined 1

__HEADER_END

#endif
