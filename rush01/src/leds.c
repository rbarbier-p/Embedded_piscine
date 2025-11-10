#include "leds.h"

void	ledb_binary_display(uint8_t value)
{
	PORTB &= 0b11101000; //clear the led on portb using mask

	PORTB |= value & 0b111;
	PORTB |= (value & 0b1000) << 1;
	//write to portb only the relevant bits of value with appropriate offsets
}

void	set_led_d9(uint8_t value)
{
}

void	set_led_d10(uint8_t value)
{
}
