#include "lib_spi.h"
#include <avr/interrupt.h>

void	SPI_send_led_start(void)
{
	SPI_master_send(0x0); //starting led frame is 32bit of 0 (p3)
	SPI_master_send(0x0);
	SPI_master_send(0x0);
	SPI_master_send(0x0);
}

void	SPI_send_led_frame(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
	SPI_master_send(0xE0 | brightness & 0x1F);//send global bit and 5bit value for brightness
	SPI_master_send(b);
	SPI_master_send(g);
	SPI_master_send(r);
}

void	SPI_send_led_stop(void)
{
	SPI_master_send(0xFF); //end frame is 32 bits of 1 (p3)
	SPI_master_send(0xFF);
	SPI_master_send(0xFF);
	SPI_master_send(0xFF);
}

void	SPI_set_3led(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
	SPI_send_led_start();
	SPI_send_led_frame(r, g, b, brightness);
	SPI_send_led_frame(r, g, b, brightness);
	SPI_send_led_frame(r, g, b, brightness);
	SPI_send_led_stop();
}
