#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t direction = 1;

void init_timer0(void) 
{
	TCCR0A |= (1 << WGM01); // CTC mode
	TCCR0B |= (1 << CS02) | (1 << CS00); // 1024 prescaler
	TIMSK0 |= (1 << OCIE0A); // ouput compare match A interrupt enable

	OCR0A = 255; // 0.016384s per timer0 cycle. / 1(s) = ~61
}

void init_timer1(void)
{
	TCCR1A |= (1 << WGM11) | (1 << COM1A1); //PWD mode, clar on compare, set at BOTTOm
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10); // no prescalar

	ICR1 = 30000;
	OCR1A = 0;
}

ISR(TIMER0_COMPA_vect)
{
	if (direction == 1) {
		if (OCR1A < 30000)
			OCR1A += 1000;
		else
			direction = 0;
	}
	if (direction == 0) {
		if (OCR1A > 0)
			OCR1A -= 1000;
		else 
			direction = 1;
	}
}

int main(void)
{
	DDRB |= (1 << PB1); // set PB1 as output

	init_timer0();
	init_timer1();

	sei();
	
	while (42);
}