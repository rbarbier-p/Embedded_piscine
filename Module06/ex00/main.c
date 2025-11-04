#include <avr/io.h>
#include <util/twi.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define AHT20_ADDRESS 0x38

char* int_to_hex(uint8_t value)
{
	static char hex_str[7];
	const char hex_chars[] = "0123456789ABCDEF";

	hex_str[0] = '0';
	hex_str[1] = 'x';

	hex_str[2] = hex_chars[value / 16];
	hex_str[3] = hex_chars[value % 16];

	hex_str[4] = '\n';
	hex_str[5] = '\r';
	hex_str[6] = '\0';

	return hex_str;
}

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++) {
		while (!(UCSR0A & (1 << UDRE0)));
    		UDR0 = str[i];
	}
}

void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	uint8_t status = TWSR & TW_STATUS_MASK;
	uart_print(int_to_hex(status));
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO)); // wait until STOP is completed
}

void i2c_init(void) {
	DDRC &= ~((1 << PC4) | (1 << PC5));
    PORTC |= (1 << PC4) | (1 << PC5);   // enable pull-ups

	TWSR = 0x00; // Prescaler value of 1
	TWBR = 72;   // 100kHz -> SCL frequency = F_CPU / (16 + 2 * TWBR * prescaler)
}

void i2c_send_address(uint8_t address, uint8_t rw) {
	TWDR = (address << 1) | rw;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	uint8_t status = TWSR & TW_STATUS_MASK;
	uart_print(int_to_hex(status));
}

void main() 
{
	i2c_init();
	uart_init();

	i2c_start();
	i2c_send_address(AHT20_ADDRESS, TW_WRITE);
	i2c_stop();

	i2c_start();
	i2c_send_address(AHT20_ADDRESS, TW_WRITE);
	i2c_stop();
}

// 0x08 START transmitted
// 0x18 SLA+W transmitted, ACK received