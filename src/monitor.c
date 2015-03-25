#include "monitor.h"
#include "isr.h"
#include "stdarg.h"

int cursor_x = 0, cursor_y = 0;
uchar vga_color;

unsigned short* vga_buffer = ((unsigned short*) 0xB8000);

static void update_cursor(){
	unsigned short location = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}

static void scroll(){
	unsigned char attribute = 15;
	unsigned short empty = 0x20 | (attribute << 8);

	if(cursor_y >= 25){
		int i;
		for(i = 0; i < 25 * 80; i++)
			vga_buffer[i] = vga_buffer[i + 80];

		for(i = 24 * 80; i < 25 * 80; i++)
			vga_buffer[i] = vga_buffer[i + 80];

		cursor_y = 24;
	}
}

void vga_putc(char c){
	unsigned short attribute = vga_color << 8;
	unsigned short* location;

	if(c == 0x08 && cursor_x)
		cursor_x--;
	else if(c == 0x09)
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	else if(c == '\r')
		cursor_x = 0;
	else if(c == '\n'){
		cursor_x = 0;
		cursor_y++;
	}
	else if(c >= ' '){
		location = vga_buffer + (cursor_y * 80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	if(cursor_x >= 80){
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	update_cursor();
}

void vga_puts(char* s){
	int i = 0;
	
	while(s[i])
		vga_putc(s[i++]);
}

void vga_puts_hex(int n){
	char* reference = "0123456789ABCDEF";
	
	vga_puts("0x");

	int i;
	for(i = 28; i > -1; i -= 4)
		vga_putc(reference[(n >> i) & 0xF]);
}

void vga_putc_hex(char n){
	char* reference = "0123456789ABCDEF";
	
	int i;
	for(i = 4; i > -1; i -= 4)
		vga_putc(reference[(n >> i) & 0xF]);
}

void vga_clear(){
	unsigned short empty = 0x20 | (15 << 8);

	int i;
	for(i = 0; i < 80 * 25; i++)
		vga_buffer[i] = empty;

	cursor_x = cursor_y = 0;
	update_cursor();
}

void vga_set_fg(uchar color){
	vga_color &= 0xF0;
	vga_color |= color;
}

void vga_set_bg(uchar color){
	vga_color &= 0xF;
	vga_color |= (color << 4);
}

void vga_blink(){
	vga_color |= 0x80;
}

void vga_reset(){
	vga_set_fg(WHITE);
	vga_set_bg(BLACK);
}

void vga_puts_dec(int n){
	if(n == 0){
		vga_putc('0');
		return;
	}
	
	if(n < 0){
		vga_putc('-');
		n *= -1;
	}

	char output[20];	

	int end = 0;
	while(n != 0){
		char digit = n % 10;
		output[end++] = '0' + digit;
		n /= 10;
	}

	while(end > 0)
		vga_putc(output[--end]);
}

void vga_fmt(const char* fmt, ...){
	va_list params;
	va_start(params, fmt);

	int arg = 0;
	while(*fmt != '\0'){
		if(*fmt == '%'){
			char op = *(++fmt);
			switch(op){
				case 'd':
				case 'i':
					vga_puts_dec(va_arg(params, int));
					break;
				case 'x':
				case 'X':
					vga_puts_hex(va_arg(params, uint));
					break;
				case 's':
					vga_puts(va_arg(params, char*));
					break;

				default:
					WARN("bad specifier");
					return;
			}
		}

		else
			vga_putc(*fmt);

		fmt++;
	}
}
