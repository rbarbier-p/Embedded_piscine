#include <avr/io.h>

int main(void) {
	DDRB |= (1 << PB1); // Set OC1A (PB1) as output

    TCCR1A = (1 << COM1A0); // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS12); // set timer1 to CTC mode, prescaler = 256

    OCR1A = 31250; // set matching value
	
    while (42);
}

/*
Timer1 is a 16 bit timer, counting 0 -> 65535

F_CPU = 16 MHz (16 mill CPU cycles per second)
Time per CPU cyle = 1 / 16000000 = 62.5 nanoseconds
Time per timer tick = 62.5 ns x prescaler -> 62.5 ns x 256 = 16 microseconds
OCR1A = timer time / time per tick -> 0.5 s / 0.000016 s = 31250
*/
