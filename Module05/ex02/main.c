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
	ADMUX |= (1 << REFS0); // set reference AVCC and left adjust ADC result
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128 for 125kHz
	ADCSRA |= (1 << ADEN); // enable ADC
}

uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0b11110000) | (channel & 0b00001111); // Select the ADC channel (0-7)
	ADCSRA |= (1 << ADSC);// Start the conversion

	while (ADCSRA & (1 << ADSC)); // Wait for the conversion to complete

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

void main(void)
{
	uint16_t adc_value;

	ADC_init();
	uart_init();
	
	while (1)
	{
		adc_value = ADC_read(0);
		uart_print(int_to_str(adc_value));
		uart_print(", ");
		adc_value = ADC_read(1);
		uart_print(int_to_str(adc_value));
		uart_print(", ");
		adc_value = ADC_read(2);
		uart_print(int_to_str(adc_value));
		uart_print("\n\r");
		_delay_ms(20);
	}
}