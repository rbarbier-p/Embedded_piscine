#include <avr/io.h>

int main (void) {
	DDRB |= (1 << PB1); // set PB1 as output

	while (1)
	{
		PORTB ^= (1 << PB1); // switch its state
		for (uint32_t i = 0; i < 1300000; i++);
	}
}