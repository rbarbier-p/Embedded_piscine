#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    int f_did_change = 0;

    DDRB |= (1 << PB0); // set PB0 as output
    DDRD &= ~(1 << PD2); // set PD2 as input

    while (1)
    {
        if (!(PIND & (1 << PD2)) && !f_did_change) // change the state if button is pressed
        {
            PORTB ^= (1 << PB0); // toggle bit
            f_did_change = 1;
        }
        else if ((PIND & (1 << PD2)) && f_did_change) // reset the flag is button is released
            f_did_change = 0;

        _delay_ms(50); // avoid bounce effects
    }
} 