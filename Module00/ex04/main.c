#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    int f_flipflop_A = 0;
    int f_flipflop_B = 0;
    int number = 0;

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // set LEDs as outputs
    DDRD &= ~((1 << PD2) | (1 << PD4)); //set buttons as inputs

    while (1)
    {
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)); // clear the output

        if (!(PIND & (1 << PD2)) && !f_flipflop_A) // increment if SW1 is pressed
        {
            if (number < 15)
                number += 1;
            f_flipflop_A = 1;
        }
        else if ((PIND & (1 << PD2)) && f_flipflop_A) // reset flipflop when released
            f_flipflop_A = 0;
        
        if (!(PIND & (1 << PD4)) && !f_flipflop_B) // decrement if SW2 is pressed
        {
            if (number > 0)
                number -= 1;
            f_flipflop_B = 1;
        }
        else if ((PIND & (1 << PD4)) && f_flipflop_B) // reset flipflop when released
            f_flipflop_B = 0;

        PORTB |= (number & 0x07); // set the first 3 bits of the number 

        if (number & (1 << 3)) // check if the 4th bit is 1, then set it in PB4
            PORTB |= (1 << PB4);

        _delay_ms(50); // avoid bounce effects
    }
}
