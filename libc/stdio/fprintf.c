#include "stdio.h"
#include "stdarg.h"

void print_decimal(FILE* f, int n);
void print_hex(FILE* f, int n);

int fprintf(FILE* f, const char* fmt, ...){
	va_list params;
	va_start(params, fmt);
	
	int arg = 0;
	while(*fmt != '\0'){
		if(*fmt == '%'){
			fmt++;
			char op = *fmt;

			switch(op){
				case 'd':
				case 'i':
					print_decimal(f, va_arg(params, int));
					break;

				case 'x':
				case 'X':
					print_hex(f, va_arg(params, int));
					break;

				case 'c':
					fputchar(f, va_arg(params, int));
					break;

				case 's':
					fprint(f, va_arg(params, char*));
					break;

				default:
					return -1;
			}
		}

		else
			fputchar(f, *fmt);

		fmt++;
	}

	return 0;
}

void print_decimal(FILE* f, int n){
	if(n == 0){
		fputchar(f, '0');
		return;
	}

	if(n < 0){
		fputchar(f, '-');
		n *= -1;
	}

	char output[20];
	int end = 0;
	while(n != 0){
		char digit = n % 10;
		output[end++] = '0' + digit;
		n /= 10;
	}

	while(end >= 0)
		fputchar(f, output[--end]);
}

void print_hex(FILE* f, int n){
	char* hex_tab = "0123456789ABCDEF";
	char* s = (char*) &n;
	int i;
	for(i = 4; i >= 0; i--){
		fputchar(f, hex_tab[(s[i] & 0xF0) >> 4]);
		fputchar(f, hex_tab[s[i] & 0x0F]);
	}
}
