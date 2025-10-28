#include <avr/io.h>

int main(void) {
	DDRB |= (1 << PB1); // Set OC1A (PB1) as output

    TCCR1A = (1 << COM1A1) | (1 << WGM11); // clear OC1A on compare match
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12); // set timer1 to fast PWM mode, prescaler = 256

    ICR1 = 62500; // TOP value
    OCR1A = 6250; // compare value (10% duty cycle)
	
    while (42);
}
