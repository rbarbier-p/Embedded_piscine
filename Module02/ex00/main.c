#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#include <avr/io.h>
#include <util/delay.h>

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0); // set double-speed
	UBRR0L = (unsigned char)ubrr; // set baud rate 

	UCSR0B |= (1 << TXEN0); // enable the USART transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set character size (8-bit)
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))); // wait for empty transmit buffer
    
    UDR0 = c; // put data into buffer, sends the data
}

int main(void)
{
    uart_init();

    while (1)
    {
        uart_tx('Z');
        _delay_ms(1000);
    }
}

// UART frame format: [start bit][data bits][parity bit (optional)][stop bit(s)]
// 8N1 -> 8 data bits, no parity bits, 1 stop bit