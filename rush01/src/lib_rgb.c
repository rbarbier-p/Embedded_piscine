#include "lib_rgb.h"

void	init_rgb(void)
{
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	TCCR0A |= 1 << WGM01 | 1 << WGM00; // set timer0 to fast pwm mode ; top is 0xFF (p115)
	TCCR0B |= 1 << CS00; //set clock souce ; no prescaler (p117)
	TCCR0A |= 1 << COM0A1 | 1 << COM0B1; // set comparator to turn pin OC0A and OC0B off on match for timer0 (p113)

	TCCR2A |= 1 << WGM21 | 1 << WGM20; // set timer0 to fast pwm mode ; top is 0xFF (p164)
	TCCR2B |= 1 << CS20; //set clock souce ; no prescaler (p165)
	TCCR2A |= 1 << COM2A1 | 1 << COM2B1; // set comparator to turn pin OC0A and OC0B off on match for timer0 (p163)
}

void	init_unicolor_rgb(void)
{
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	//since timer0 and timer2 are 8 bit, max value is also 255;
	//as per pdf, OC0B is red, OC0A is green and OC2B is blue
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
	//value are turned on for only a part of a cycle of 255 ; since values are 0-255, it works as a percentage of time and pwm work without math
}

void	set_unicolor_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	if (r)
	{
		PORTD |= 1 << PD5;
		PORTD &= (1 << PD3 | 1 << PD6) ^ 0xFF;
	}
	else if (g)
	{
		PORTD |= 1 << PD6;
		PORTD &= (1 << PD3 | 1 << PD5) ^ 0xFF;
	}
	else
	{
		PORTD |= 1 << PD3;
		PORTD &= (1 << PD5 | 1 << PD6) ^ 0xFF;
	}
}

void wheel(uint8_t pos) { //function given by subject
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void	disable_rgb(void) //reset timer configuration values to their initial values
{
	TCCR0A = 0;
	TCCR0B = 0;

	TCCR2A = 0;
	TCCR2B = 0;

	OCR0A = 0;
	OCR0B = 0;
	OCR2B = 0;
}

void	disable_unicolor_rgb(void)
{
	PORTD &= (1 << PD3 | 1 << PD5 | 1 << PD6) ^ 0xFF;
	DDRD &= (1 << PD3 | 1 << PD5 | 1 << PD6) ^ 0xFF;
}
