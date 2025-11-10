
//based on https://www.superlightingled.com/PDF/APA102.pdf

#ifndef LIB_SPI_LED_H
# define LIB_SPI_LED_H
#include "lib_spi.h"
#include <avr/interrupt.h>

void	SPI_send_led_start(void);
void	SPI_send_led_frame(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
void	SPI_send_led_stop(void);

void	SPI_set_3led(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
#endif
