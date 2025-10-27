#include <avr/io.h>

int main(void) {
    DDRB |= (1 << PB0); // set PB0 as output
    PORTB |= (1 << PB0); // set PB0 HIGH
}