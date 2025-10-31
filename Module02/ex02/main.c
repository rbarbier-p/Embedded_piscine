#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#include <avr/io.h>

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0); // set double-speed
	UBRR0L = (unsigned char)ubrr; // set baud rate 

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable the USART transmitter and reciver
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set character size (8-bit)
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))); // wait for empty transmit buffer
    
    UDR0 = c; // put data into buffer, sends the data
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))); // wait for unread data in buffer

    return UDR0; // read data from reciver buffer
}

int main(void)
{
    uart_init();

    while (1)
    {
        uart_tx(uart_rx());
    }
}