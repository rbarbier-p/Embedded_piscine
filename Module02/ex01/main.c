#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0); // set double-speed
	UBRR0L = (unsigned char)ubrr; // set baud rate 

	UCSR0B |= (1 << TXEN0); // enable the USART transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set character size (8-bit)
}

ISR(TIMER1_COMPA_vect) {
    char message[] = "Hello World!\n\r";
    for (uint8_t i = 0; i < sizeof(message) - 1; i++) {
        while (!(UCSR0A & (1 << UDRE0))) {} //wait until UDR0 is ready
        UDR0 = message[i]; // send byte
    }
}

int main(void)
{
    uart_init();

    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // set timer1 to CTC mode, prescaler = 1024
    TIMSK1 |= (1 << OCIE1A); // enable timer1 compare interrupt 

    OCR1A = 32048; // set matching value (2s)

    sei(); // enables interrupts

    while (42);
}