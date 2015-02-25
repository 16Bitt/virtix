#include "stdio.h"
#include "stdarg.h"

int printf(const char* fmt, ...){
	va_list params;
	va_start(params, fmt);

	while(*fmt != '\0'){
		if(*fmt == '%'){
			fmt++;
			char op = *fmt;
			switch(op){
				case 'x':
				case 'X':
					fprintf(stdout, "%x", va_arg(params, int));
					break;

				case 'i':
				case 'd':
					fprintf(stdout, "%d", va_arg(params, int));
					break;

				case 'c':
					putchar(va_arg(params, int));
					break;

				case 's':
					print(va_arg(params, char*));
					break;

				default:
					return -1;
			}
		}

		else
			putchar(*fmt);

		fmt++;
	}

	return;
}
