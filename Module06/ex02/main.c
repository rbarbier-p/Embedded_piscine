#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdlib.h>

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

void uart_print_dec(uint16_t value)
{
	char buffer[6];
	uint8_t i = 0;

	if (value == 0) {
		uart_print("0");
		return;
	}

	while (value > 0) {
		buffer[i++] = (value % 10) + '0';
		value /= 10;
	}

	for (int8_t j = i - 1; j >= 0; j--) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = buffer[j];
	}
}
void print_values(double humidity, double temperature)
{
	char buf[6];

	uart_print("Temperature: ");
	dtostrf(temperature, 4, 1, buf);
	uart_print(buf);
	uart_print("°C, Humidity: ");
	dtostrf(humidity, 4, 1, buf);
	uart_print(buf);
	uart_print("%\r\n");
}

void i2c_read(double *humidity, double *temperature)
{
	static uint8_t data[6];
	
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
	} while (status & 0x10000000 == 1);

    i2c_start();
    i2c_SLA_R();
	
	uart_print(".");

	for (uint8_t i = 0; i < 5; i++)
		data[i] = i2c_read_TWDR(ACK);
	data[5] = i2c_read_TWDR(NACK);
	i2c_stop();

	uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3]) >> 4);
	uint32_t raw_temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | (data[5]);

	*humidity += (raw_humidity / 1048576.0) * 100.0;
	*temperature += ((raw_temperature / 1048576.0) * 200.0) - 50.0;
}

void main() 
{
	uart_init();
	i2c_init();

	double humidity;
	double temperature;

	while (1)
	{
		humidity = 0;
		temperature =0;
		
		i2c_read(&humidity, &temperature);
		_delay_ms(2000);
		i2c_read(&humidity, &temperature);
		_delay_ms(2000);
		i2c_read(&humidity, &temperature);
		
		humidity /= 3.0;
		temperature /= 3.0;

		print_values(humidity, temperature);

		_delay_ms(2000);
	}
}