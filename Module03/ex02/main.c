#include <avr/io.h>
#include <util/delay.h>

void init_timer()
{
    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B |= (1 << CS00);

    TCCR2A |= (1 << WGM20) | (1 << COM2B1);
    TCCR2B |= (1 << CS20);
}

void init_rgb() {
    DDRD |= (1 << PD3); // blue
    DDRD |= (1 << PD5); // red
    DDRD |= (1 << PD6); // green
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) 
{
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}


int main(void) {
    init_rgb();
    init_timer();

    while (42) {
        for (uint8_t i = 0; i < 255; i++) {
            wheel(i);
            _delay_ms(20);
        }
    }
}