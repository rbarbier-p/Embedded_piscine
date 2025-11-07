#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = c;
}

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++)
		uart_tx(str[i]);
}

void print_hex_value(uint16_t c, uint8_t digits)
{
	const char hex_chars[] = "0123456789abcdef";
	if (digits == 3) uart_tx(hex_chars[(c >> 8) & 0x0F]);
	uart_tx(hex_chars[(c >> 4) & 0x0F]);
	uart_tx(hex_chars[c & 0x0F]);
}

uint8_t EEPROM_read(uint16_t addr)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address register */
	EEARH = (addr >> 8) & 0x03;
	EEARL = addr; // 10 bit address
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

void main() 
{
	uart_init();

	for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            uart_print("\r\n");
			uart_print("00000");
            print_hex_value(i, 3);
            uart_print(": ");
        }

        print_hex_value(EEPROM_read(i), 2);
		if (i % 2 != 0)
        	uart_print(" ");
    }
}