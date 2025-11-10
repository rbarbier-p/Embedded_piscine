#include "lib_adc.h"

void	adc_init(void)
{
	ADCSRA |= 1 << ADEN; //enable adc (p258)
	ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0; //set prescaler to 128 (p259)
}

void	adc_set_ref_AVCC(void)
{
	ADMUX &= (1 << REFS0 | 1 << REFS1) ^ 0xFF; //make sure ref is reset
	ADMUX |= 1 << REFS0; //set ref to Avcc (p258)
}

void	adc_set_channel(uint8_t channel)
{
	ADMUX = (ADMUX & 0b11110000) | (channel & 0b1111); //set ADMUX to it's previous value and first 4 bits of channel;
}

uint8_t	adc_read_8bit(void)
{
	ADMUX |= 1 << ADLAR; //align read data to the left to ensure correct return value
	ADCSRA |= 1 << ADSC; //trigger adc (p258)

	while (ADCSRA & 1 << ADSC); //block until adc has read
	return (ADCH);
}

uint16_t	adc_read_10bit(void)
{
	ADMUX &= 1 << ADLAR ^ 0xFF; //align data to the right to ensure correct return value
	ADCSRA |= 1 << ADSC; //trigger adc (p258)

	while (ADCSRA & 1 << ADSC); //block until adc has read
	const uint8_t result = ADCL;
	return (result | ADCH << 8);
}

void	adc_init_temperature(void)
{
	ADMUX &= (1 << REFS0 | 1 << REFS1) ^ 0xFF; //make sure ref is reset
	ADMUX |= 1 << REFS0 | 1 << REFS1; //set ref to internal 1.1v (p256/257)
	adc_set_channel(8); //internal temp sensor is 8th channel (p256)
}

uint32_t	adc_read_temperature(void)
{
	return (adc_read_10bit() * 0.78125 - 250); // linear function derived from p256-24.8
}

void	adc_disable(void)
{
	ADCSRA = 0;
}
