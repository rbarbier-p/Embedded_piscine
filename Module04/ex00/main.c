#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect)
{
	if (!(EICRA & (1 << ISC00))) {// if in falling edge
		PORTB ^= (1 << PB0); // toggle led
		_delay_ms(20);

		EICRA |= (1 << ISC00); // switch to rising mode

		EIFR |= (1 << INTF0); // clear any interrupts pending
	}
	else if ((EICRA & (1 << ISC00))) { // if in any change
		_delay_ms(20);

		EICRA |= (1 << ISC01);
		EICRA &= ~(1 << ISC00); // back to falling edge

		EIFR |= (1 << INTF0); // clear any interrupts pending
	}
}

int main(void) {
	DDRB |= (1 << PB0); // led as output
	DDRD &= ~(1 << PD2);   // PD2 as input (INT0)
	PORTD |= (1 << PD2);   // Enable pull-up resistor on PD2

	EICRA = (1 << ISC01); // falling edge (INT0)
	EIMSK |= (1 << INT0); // Enable external interrupt 0

	sei();

	while (42);
}