#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t number = 0;

void update_display() 
{
    PORTB = (number & 0x07);

    if (number & (1 << 3))
        PORTB |= (1 << PB4);
}

ISR(INT0_vect)
{
	if (!(EICRA & (1 << ISC00))) {// if in falling edge
		if (number < 15)
            number++;

        update_display();

		_delay_ms(20);

		EICRA |= (1 << ISC00); // switch to rising mode
		EIFR |= (1 << INTF0); // clear any interrupts pending
	}
	else if ((EICRA & (1 << ISC00))) { // if in any change
		_delay_ms(20);

		EICRA |= (1 << ISC01);// back to falling edge
		EICRA &= ~(1 << ISC00); 
        
		EIFR |= (1 << INTF0); // clear any interrupts pending
	}
}

ISR(PCINT2_vect)
{
    static int pressed = 0;
    pressed = (pressed + 1) % 2; //skips one pin change interrupt (when releasing)

    if (pressed == 1 && number > 0)
    {
        number--;
        update_display();
    }

    _delay_ms(20);
    PCIFR |= (1 << PCIF2); // clear any pending interrupts
}

int main(void) {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // set leds as outputs
    DDRD &= ~((1 << PD2) | (1 << PD4)); // set buttons as inputs
    PORTD |= (1 << PD2) | (1 << PD4); // set pullup resistors for buttons

    EICRA = (1 << ISC01); // falling edge (INT0)
	EIMSK |= (1 << INT0); // Enable external interrupt 0

    PCICR |= (1 << PCIE2); // enable Pin CHange Interrupt group for PORTD
    PCMSK2 |= (1 << PCINT20); // enable interrupt for PD4 specifically

	sei();

    while (42);
}
