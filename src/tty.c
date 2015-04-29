#include "common.h"
#include "monitor.h"
#include "tty.h"
#include "isr.h"

extern void scroll();
extern void update_cursor();
extern int cursor_x, cursor_y;

uchar tty_fg = WHITE;
uchar tty_bg = BLACK; 
short tty_row;
short tty_col;

bool tty_echo = true;
bool tty_esc_mode = false;

char tty_key = 0;

char esc_buf[256];
int esc_index = 0;

int tty_parse_int(char* s){
	int x = 0;
	while((*s != '\0') && (*s != ';') && (*s != 'm')){
		if((*s >= '0') && (*s <= '9')){
			x *= 10;
			x += *s - '0';
		}
		s++;
	}

	return x;
}

int tty_ansi_to_vga(int color, int mode){
	if(mode)
		switch(color){
			case 30:

			case 34:
				return LIGHT_BLUE;
			case 32:
				return LIGHT_GREEN;
			case 36:
				return LIGHT_CYAN;
			case 31:
				return LIGHT_RED;
			case 35:
				return LIGHT_MAGENTA;
			case 33:
				return LIGHT_BROWN;
			case 37:
				return WHITE;
			default:
				return WHITE;
		}

	else
		switch(color){
			case 30:
				return BLACK;
			case 34:
				return BLUE;
			case 32:
				return GREEN;
			case 36:
				return CYAN;
			case 31:
				return RED;
			case 35:
				return MAGENTA;
			case 33:
				return BROWN;
			case 37:
				return LIGHT_GREY;
			default:
				return WHITE;
		}
}

void tty_parse_escape(){
	int i = 0;
	if(esc_buf[i] == '['){
		i++;
	}
	
	int command = tty_parse_int(&esc_buf[i]);
	int args[20]; //Possible security problem...
	memset(args, 0, sizeof(args));
	int arg = 0;

	while(esc_buf[i] != '\0'){
		while(esc_buf[i] != ';'){
			if(esc_buf[i] == '\0')
				break;
			i++;
		}
		i++;

		args[arg++] = tty_parse_int(&esc_buf[i]);
	}

	switch(command){
		case 0:	//Basic colors
			tty_fg = tty_ansi_to_vga(args[0], 0);
			break;
		case 1: //Other colors
			tty_fg = tty_ansi_to_vga(args[0], 1);
			break;

		default:
			WARN("Unknown vt220 mode");
	}
}

void add_to_escape(char c){
	if(c == 'm'){
		esc_index = 0;
		tty_parse_escape();
		memset(esc_buf, 0, sizeof(esc_buf));
		tty_esc_mode = false;
		return;
	}

	esc_buf[esc_index++]  = c;
	if(esc_index >= 255)
		esc_index = 0;
}

void tty_clear(){
	vga_clear();
	tty_row = 0;
	tty_col = 0;
}

void tty_reset(){
	tty_clear();
	tty_bg = BLACK;
	tty_fg = WHITE;
	tty_echo = true;
}

void tty_backspace(){
	if(tty_col == 0)
		if(tty_row > 0){
			tty_row--;
			cursor_y--;
			tty_col = TTY_WIDTH - 1;
			cursor_x = tty_col;
			update_cursor();
		}
	else
		vga_putc(0x08);
}

void tty_putc(char c){
	switch(c){
		case 27:	//Escape
			tty_esc_mode = true;
			break;
		case 8:		//Backspace
			tty_backspace();
			break;
		default:
			if(tty_esc_mode)
				add_to_escape(c);
			else if(tty_echo){
				vga_set_fg(tty_fg);
				vga_set_bg(tty_bg);
				vga_putc(c);
			}
	}
}
