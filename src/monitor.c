#include "monitor.h"

int cursor_x = 0, cursor_y = 0;

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
	unsigned short attribute = 15 << 8;
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

void vga_clear(){
	unsigned short empty = 0x20 | (15 << 8);

	int i;
	for(i = 0; i < 80 * 25; i++)
		vga_buffer[i] = empty;

	cursor_x = cursor_y = 0;
	update_cursor();
}
