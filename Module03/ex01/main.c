#include <avr/io.h>
#include <util/delay.h>

#define DELAY_MS 1000

int main(void) {
    DDRD |= (1 << PD3); // blue
    DDRD |= (1 << PD5); // red
    DDRD |= (1 << PD6); // green

    while (42) {
        PORTD = (1 << PD5);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD6);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD3);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD5) | (1 << PD6);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD6) | (1 << PD3);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD3) | (1 << PD5);
        _delay_ms(DELAY_MS);
        PORTD = (1 << PD3) | (1 << PD5) | (1 << PD6);
        _delay_ms(DELAY_MS);
    }
}