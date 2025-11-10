//based on https://www.nxp.com/docs/en/data-sheet/PCF8563.pdf

#ifndef LIB_RTC_H
# define LIB_RTC_H

#include "I2C.h"
#include <avr/io.h>

//p23
#define RTC_ADDR	0x51

#define RTC_YEARS_REG	0x08
#define RTC_DAYS_REG	0x5
#define RTC_HOURS_REG	0x4
#define RTC_MINUTES_REG	0x3
#define RTC_SEC_REG	0x2

#define _CONVERT(value, digit_bit_place, ten_bit_place)	value & (1 << digit_max_bit + 1 ^ 0xFF) \
	+ (value & (1 << ten_max_bit_place + 1 ^ 0xFF) * 10)

#define CONVERT_TO_SEC(n)	((n & 0b1110000) >> 4) * 10 + n & 0b1111 
#define CONVERT_TO_MIN(n)	CONVERT_TO_SEC(n)
#define CONVERT_TO_HOURS(n)	((n & 0b110000) >> 4) * 10 + n & 0b1111
#define CONVERT_TO_DAYS(n)	CONVERT_TO_HOURS(n)
#define CONVERT_TO_MONTH(n)	((n & 0b10000) >> 4) * 10 + n & 0b1111
#define	CONVERT_TO_YEARS(n)	((n & 0b11110000) >> 4) * 10 + n & 0b1111

void	RTC_init(void);
void	RTC_get_time(uint8_t *result);
void	RTC_set_time(uint8_t *date, uint8_t *time);

#endif
