#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define ANSI_COLOR_RED     "\x1b[31m"

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))); // wait for unread data in buffer

    return UDR0; // read data from reciver buffer
}

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++) {
		while (!(UCSR0A & (1 << UDRE0)));
    		UDR0 = str[i];
	}
}

void print_hex_value(char c)
{
	static char hex_str[4];
	const char hex_chars[] = "0123456789abcdef";
	uint8_t n = (uint8_t) c;

	hex_str[0] = hex_chars[n / 16];
	hex_str[1] = hex_chars[n % 16];
	hex_str[3] = '\0';

	uart_print(hex_str);
}

unsigned char EEPROM_read(uint16_t addr)
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

void EEPROM_write(uint16_t addr, uint8_t data)
{
	/* Wait for completion of previous write */
	if (EEPROM_read(addr) == data)
		return (uart_print("data already there, no write\n\r"), (void)0);
	/* Set up address and Data Registers */
	EEARH = (addr >> 8) & 0x03;
	EEARL = addr;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

void EEPROM_hexdump_highlight(uint8_t addr, uint8_t data) 
{
	for (uint16_t i = 0; i < 1024; i++) {
		if (EEPROM_read(i) == data);
			return ;			
	}
	for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            uart_print("\r\n");
			uart_print("000000");
            print_hex_value(i);   // print address
            uart_print(": ");
        }
		if (EEPROM_read(i) == data) {
			uart_print("\x1b[31m");
			print_hex_value(EEPROM_read(i));
			uart_print("\x1b[0m");
		} else
			print_hex_value(EEPROM_read(i));
		
		if (i % 2 != 0)
        	uart_print(" ");
    }
}

void main() {
	uart_init();

	//EEPROM_write(0, 0x08);
	

	while (1) {
		get_input
	}
}