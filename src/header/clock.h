#ifndef CLOCK_H
#define CLOCK_H

#include "common.h"
#include "isr.h"

typedef void (*clock_handler_t) (registers_t* regs);

void start_timer(unsigned int frequency, clock_handler_t handler);
void init_virtix_clock();
uint get_time();
void sleep(uint seconds);
void msleep(uint mseconds);
void init_unix_time();

uint get_year();
uint get_month();
uint get_day();
uint get_hour();
uint get_minute();
uint get_seconds();

#define CMOS_CMD	0x70
#define CMOS_DATA	0x71
#define CMOS_CMD_STAT	0x0A
#define RTC_SECOND	0x00
#define RTC_MINUTE	0x02
#define RTC_HOUR	0x04
#define RTC_DAY		0x07
#define RTC_MONTH	0x08
#define RTC_YEAR	0x09
#define RTC_REG_A	0x0A
#define RTC_REG_B	0x0B

#define RTC_TIME_24	2
#define RTC_MODE_BINARY	4

#endif
