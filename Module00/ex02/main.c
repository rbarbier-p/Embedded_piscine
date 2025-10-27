#include <avr/io.h>

int main(void) {
    DDRB |= (1 << PB0); // set PB0 as an output
    DDRD |= (0 << PD2); // set PD2 as an input

    while (1)
    {
        if (!(PIND & (1 << PD2))) //if button pressed, set to 1
            PORTB |= (1 << PB0);
        else
            PORTB &= ~(1 << PB0);
    }
}