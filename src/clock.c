#include "common.h"
#include "clock.h"
#include "isr.h"
#include "monitor.h"

uint unix_time = 0;
uint virtix_time = 0;
uint time_div = 0;
uint virtix_mseconds = 0;

void virtix_clock(registers_t* regs){
	time_div++;
	if(time_div == 100){
		time_div = 0;
		virtix_time++;
	}
}

void start_timer(unsigned int frequency, clock_handler_t handler){
	register_interrupt_handler(32, handler);
	
	unsigned int divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	unsigned char low	= (unsigned char) (divisor & 0xFF);
	unsigned char high	= (unsigned char) ((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
}

void init_virtix_clock(){
	start_timer(1, virtix_clock);
	init_unix_time();
}

uint get_time(){
	return virtix_time + unix_time;
}

uchar cmos_register(int reg){
	outb(CMOS_CMD, reg);
	return inb(CMOS_DATA);
}

uchar bcd_to_bin(int value){
	return (value & 0xF) + 10 * ((value >> 4) & 0xF);
}

void init_unix_time(){
	outb(CMOS_CMD, RTC_REG_B);
	int flags = inb(CMOS_DATA);
	int second, minute, hour, day, month, year;

	second	= cmos_register(RTC_SECOND);
	minute	= cmos_register(RTC_MINUTE);
	hour	= cmos_register(RTC_HOUR);
	day	= cmos_register(RTC_DAY);
	month	= cmos_register(RTC_MONTH);
	year	= cmos_register(RTC_YEAR);
	
	if(!(flags & RTC_MODE_BINARY)){
		second	= bcd_to_bin(second);
		minute	= bcd_to_bin(minute);
		hour	= bcd_to_bin(hour);
		day	= bcd_to_bin(day);
		month	= bcd_to_bin(month);
		year	= bcd_to_bin(year);
	}
	
	if(!(flags & RTC_TIME_24)){
		hour = cmos_register(RTC_HOUR);
		if(hour & 0x80)
			hour = 12 + bcd_to_bin(hour & 70);
		else
			hour = bcd_to_bin(hour);
	}
	
	year += 2000;

	vga_fmt("%d:%d:%d %d-%d-%d\n", hour, minute, second, month, day, year);
	unix_time = ((year - 1970) * 365 * 24 * 60 * 60)\
	+ (30 * month * 24 * 60 * 60)\
	+ (24 * day * 60 * 60)\
	+ (60 * 60 * hour)\
	+ (60 * minute)\
	+ second;

	vga_fmt("UNIX time is roughly %ds\n", unix_time);
	vga_fmt("Virtix date: %d:%d:%d %d-%d-%d\n", get_hour(), get_minute(), get_seconds(), get_month(), get_day(), get_year() + 1970);
}

uint get_year(){
	return get_time() / (365 * 24 * 60 * 60);
}

uint get_month(){
	return (get_time() - (get_year() * 365 * 24 * 60 * 60)) / (30 * 24 * 60 * 60);
}

uint get_day(){
	return (get_time() - ((get_year() * 365 * 24 * 60 * 60) + (get_month() * 30 * 24 * 60 * 60)))\
	/ (24 * 60 * 60);
}

uint get_hour(){
	return (get_time()\
	- ((get_year() * 365 * 24 * 60 * 60)\
	+ (get_month() * 30 * 24 * 60 * 60)\
	+ (get_day() * 24 * 60 * 60)))\
	/ (60 * 60);
}

uint get_minute(){
	return (get_time()\
	- ((get_year() * 365 * 24 * 60 * 60)\
	+ (get_month() * 30 * 24 * 60 * 60)\
	+ (get_day() * 24 * 60 * 60)\
	+ (get_hour() * 60 * 60)))\
	/ 60;
}

uint get_seconds(){
	return (get_time()\
	- ((get_year() * 365 * 24 * 60 * 60)\
	+ (get_month() * 30 * 24 * 60 * 60)\
	+ (get_day() * 24 * 60 * 60)\
	+ (get_hour() * 60 * 60)\
	+ (get_minute() * 60)));
}

void sleep(uint seconds){
	uint start = get_seconds();
	while(get_seconds() < (start + seconds));
}
