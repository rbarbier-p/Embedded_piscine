#include "ADC_lib.h"

void ADC_init(void)
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR); // set reference AVCC and left adjust ADC result
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128 for 125kHz
	ADCSRA |= (1 << ADEN); // enable ADC
}

uint8_t ADC_read_RV1(void)
{
	ADCSRA |= (1 << ADSC);// Start the conversion

	while (ADCSRA & (1 << ADSC)); // Wait for the conversion to complete

	return ADCH;
}
