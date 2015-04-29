#ifndef BYTESWAP_H
#define BYTESWAP_H

#include "sys/cdefs.h"

__HEADER_START

#define bswap_16(x)	__builtin_bswap16((x))
#define bswap_32(x)	__builtin_bswap32((x))
#define bswap_64(x)	__builtin_bswap64((x))

__HEADER_END

#endif
