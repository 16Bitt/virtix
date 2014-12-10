//This file is simply for simple, in-kernel parsing

#ifndef STRUTIL_H
#define STRUTIL_H

#include "common.h"

//Turn all spaces and newlines into \o
char* prep_str(char* str);

//Returns the next word in a prep_str'd string
char* next_str(char* str);

//Moves a string to a given destination, delimiter and all
void strmov(char* dest, char* src);

#endif
