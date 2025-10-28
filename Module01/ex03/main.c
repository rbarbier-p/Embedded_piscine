#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    int f_press = 0;
	DDRB |= (1 << PB1); // make OC1A (PB1) output
    DDRB &= ~((1 << PD2) | (1 << PD4)); // make PD2 and PD4 inputs

    PORTD |= (1 << PD2) | (1 << PD4); // enable internal pull-ups (avoid floating inputs)

    TCCR1A = (1 << COM1A1) | (1 << WGM11); // clear OC1A on compare match
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12); // set timer1 to fast PWM mode (14), prescaler = 256

    ICR1 = 62500; // TOP value (1s)
    OCR1A = ICR1 / 10; // initial compare value (10% duty cycle)
	
    while (42) {
        if (!(PIND & (1 << PD2)) && !f_press) // increment if SW1 is pressed
        {
            if (OCR1A < ICR1)
                OCR1A += ICR1 / 10;
            f_press = 1;
        }
        if (!(PIND & (1 << PD4)) && !f_press) // decrement if SW2 is pressed
        {
            if (OCR1A > ICR1 / 10)
                OCR1A -= ICR1 / 10;
            f_press = 1;
        }
        else if ((PIND & (1 << PD4)) && (PIND & (1 << PD2)) && f_press) // reset flag when released
            f_press = 0;
        _delay_ms(50);
    }
}
