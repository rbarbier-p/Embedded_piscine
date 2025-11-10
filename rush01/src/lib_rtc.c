#include "lib_rtc.h"

void	RTC_init(void)
{
	I2C_start();
	I2C_SLA_WR(0x51, WRITE);
	I2C_write(0x00); //write to control register 0 (p6)
	I2C_write(0x00); //set all of register to 0 (normal operation) (p7)
	I2C_stop();
}

//read seconds through years in one go
//returns in result 6 uints ordered by size
void	RTC_get_time(uint8_t *result)
{
	I2C_start();
	I2C_SLA_WR(0x51, WRITE);
	I2C_write(0x02); //send 0x02 to trigger read of every register after it(p13)

	I2C_start();
	I2C_SLA_WR(0x51, READ);
	const uint8_t	sec = I2C_read(ACK); //register to read auto increments after each read (p23)
	const uint8_t	min = I2C_read(ACK);
	const uint8_t	hours = I2C_read(ACK);
	const uint8_t	days = I2C_read(ACK);
	I2C_read(ACK); //ignore weekdays
	const uint8_t	month = I2C_read(ACK);
	const uint8_t	years = I2C_read(NACK);
	I2C_stop();

	result[0] = CONVERT_TO_SEC(sec);
	result[1] = CONVERT_TO_MIN(min);
	result[2] = CONVERT_TO_HOURS(hours);
	result[3] = CONVERT_TO_DAYS(days);
	result[4] = CONVERT_TO_MONTH(month);
	result[5] = CONVERT_TO_YEARS(years);
}

void	RTC_set_time(uint8_t *date, uint8_t *time)
{
	I2C_start();
	I2C_SLA_WR(0x51, WRITE);
	I2C_write(0x02); //send 0x02 as register address to write to
	I2C_write(time[0]); //seconds
	I2C_write(time[1]); //minutes
	I2C_write(time[2]); //hours
	I2C_write(date[0]); //day
	I2C_write(0);	//weekday (ignored)
	I2C_write(date[1]); //month
	I2C_write(date[2]); //year
	I2C_stop();
}
