#ifndef CTYPE_H
#define CTYPE_H

#include "sys/cdefs.h"

__HEADER_START

#define C_BETWEEN(dw, c, up) (((dw) <= c) && (c <= (up)))

int isalnum(int c);
int iscntrl(int c);
int isspace(int c);
int isalpha(int c);
int isprint(int c);
int isupper(int c);
int islower(int c);
int isgraph(int c);
int ispunct(int c);
int isxdigit(int c);
int isdigit(int c);
int isblank(int c);

__HEADER_END

#endif
