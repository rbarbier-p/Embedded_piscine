#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define AHT20_ADDRESS 0x38
#define ACK 0
#define NACK 1

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

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO));
}


void i2c_SLA_W(void) {
	TWDR = (AHT20_ADDRESS << 1) | TW_WRITE;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_SLA_R(void) {
	TWDR = (AHT20_ADDRESS << 1) | TW_READ;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_write(unsigned char data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

char i2c_read_TWDR(char ack) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	if (ack == ACK)
		TWCR |= (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_init(void) {
	DDRC &= ~((1 << PC4) | (1 << PC5));
	PORTC |= (1 << PC4) | (1 << PC5);   // enable pull-ups
	
	TWSR = 0x00; // Prescaler value of 1
	TWBR = 72;   // 100kHz -> SCL frequency = F_CPU / (16 + 2 * TWBR * prescaler)

	_delay_ms(40);

	i2c_start();
	i2c_SLA_W();
	i2c_write(0x71);
	i2c_stop();

	i2c_start();
	i2c_SLA_R();
	i2c_read_TWDR(NACK);
	i2c_stop();

	if (!(TWDR & 0x08)) {
		i2c_start();
		i2c_SLA_W();
		i2c_write(0xBE);
		i2c_write(0x08);
		i2c_write(0x00);
		i2c_stop();
		_delay_ms(10);
	}
}

void print_hex_value(char c)
{
	static char hex_str[4];
	const char hex_chars[] = "0123456789ABCDEF";
	uint8_t n = (uint8_t) c;

	hex_str[0] = hex_chars[n / 16];
	hex_str[1] = hex_chars[n % 16];
	hex_str[2] = ' ';
	hex_str[3] = '\0';

	uart_print(hex_str);
}

void i2c_read(void)
{
	//send measurement command
	i2c_start();
	i2c_SLA_W();
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	i2c_stop();

	_delay_ms(80);

	//check that it has finished measuring
	uint8_t status;
	do {
		i2c_start();
		i2c_SLA_W();
		i2c_write(0x71);
		i2c_stop();
	 
		i2c_start();
		i2c_SLA_R();
		status = i2c_read_TWDR(NACK);
		i2c_stop();
	} while (status & 0x01000000 == 1);

	//read it
	i2c_start();
	i2c_SLA_R();
	for (uint8_t i = 0; i < 6; i++)
		print_hex_value(i2c_read_TWDR(ACK));
	print_hex_value(i2c_read_TWDR(NACK));
	uart_print("\n\r");
	i2c_stop();
}

void main() 
{
	uart_init();
	i2c_init();

	while (1)
	{
		i2c_read();
		_delay_ms(2000);
	}
}

// 0x08 START transmitted
// 0x18 MT SLA+W transmitted, ACK received
// 0x28 MT data transmitted, ACK received
// 0x30 MT data transmitted, NACK received
// 0x40 MR SLA+R transmitted, ACK received
// 0x50 MR data received, ACK returned
// 0x58 MR data received, NACK returned

/* 18 63 DF 36 19 05
void main() 
{
	uart_init();
	i2c_init();
	
	_delay_ms(40);
	
	i2c_start();
	i2c_SLA_W();
	i2c_write(0x71);
	i2c_stop();
	
	i2c_start();
	i2c_SLA_R();
	i2c_read_nack();
	i2c_stop();
	
	if (!(TWDR & 0x08)) {
		i2c_start();
		i2c_SLA_W();
		i2c_write(0xBE);
		i2c_write(0x08);
		i2c_write(0x00);
		i2c_stop();
		_delay_ms(10);
	}
	
	i2c_start();
	i2c_SLA_W();
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	i2c_stop();
	_delay_ms(80);
	
	i2c_start();
	i2c_SLA_R();
	for (uint8_t i = 0; i < 5; i++)
		i2c_read();
	i2c_read_nack();
	i2c_stop();
}
*/