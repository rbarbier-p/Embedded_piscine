#include <avr/io.h>
#include <util/delay.h>

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

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++) {
		while (!(UCSR0A & (1 << UDRE0)));
    		UDR0 = str[i];
	}
}

void ADC_init(void)
{
	ADMUX |= (1 << REFS0) | (1 << REFS1); // set reference internal 1.1V
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128 for 125kHz
	ADCSRA |= (1 << ADEN); // enable ADC
}

uint16_t ADC_read()
{
	ADMUX |= (1 << MUX3); // ADC8

	ADCSRA |= (1 << ADSC);// start the conversion

	while (ADCSRA & (1 << ADSC)); // wait for the conversion to complete

	return ADC;
}

char* int_to_str(uint16_t value)
{
	static char str[5] = {0};
	uint8_t skip_leading_zeros = 0;

	str[0] = (value / 1000) % 10 + '0';
	str[1] = (value / 100) % 10 + '0';
	str[2] = (value / 10) % 10 + '0';
	str[3] = (value % 10) + '0';

	for (uint8_t i = 0; i < 3; i++) {
		if (str[i] == '0')
			skip_leading_zeros++;
		else
			break;
	}

	return str + skip_leading_zeros;
}

uint16_t to_celcius(uint16_t adc_value)
{
	uint16_t voltage_mv = ((adc_value * 1.1) / 1024) * 1000;

	//return voltage_mv;
	return (voltage_mv * 85) / 380; //380mv = 85°C (regla de tres)
}

void main(void)
{
	uint16_t adc_value;

	ADC_init();
	uart_init();
	
	while (1)
	{
		adc_value = ADC_read();
		uart_print(int_to_str(to_celcius(adc_value)));
		uart_print("\n\r");
		_delay_ms(20);
	}
}
// ADC = (Vin * 1024) / Vref
// Vin = (ADC * Vref) / 1024
