#include "inc.h"

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

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0); // set double-speed
	UBRR0L = (unsigned char)ubrr; // set baud rate 

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable the USART transmitter and reciver
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set character size (8-bit)
}