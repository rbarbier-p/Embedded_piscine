#ifndef LEDS_H
# define LEDS_H
#include <avr/io.h>

void	ledb_binary_display(uint8_t value);

void	set_led_d9(uint8_t value);
void	set_led_d10(uint8_t value);
#endif
