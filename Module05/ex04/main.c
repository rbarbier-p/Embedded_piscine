#include <avr/io.h>
#include <util/delay.h>

void ADC_init(void)
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR); // set reference AVCC and left adjust ADC result
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128 for 125kHz
	ADCSRA |= (1 << ADEN); // enable ADC
}

uint8_t ADC_read()
{
	ADCSRA |= (1 << ADSC);// Start the conversion

	while (ADCSRA & (1 << ADSC)); // Wait for the conversion to complete

	return ADCH;
}

void digital_gauge(uint8_t value)
{
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	if (value < 64)
		PORTB = 0;
	else if (value < 128)
		PORTB = (1 << PB0);
	else if (value < 192)
		PORTB = (1 << PB1) | (1 << PB0);
	else if (value < 254)
		PORTB = (1 << PB2) | (1 << PB1) | (1 << PB0);
	else
		PORTB = (1 << PB4) | (1 << PB2) | (1 << PB1) | (1 << PB0);
}

void timer_init()
{
    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B |= (1 << CS00);

    TCCR2A |= (1 << WGM20) | (1 << COM2B1);
    TCCR2B |= (1 << CS20);
}

void rgb_init() {
    DDRD |= (1 << PD3); // blue
    DDRD |= (1 << PD5); // red
    DDRD |= (1 << PD6); // green
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) 
{
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

void main(void)
{
	uint8_t adc_value;

	ADC_init();
	rgb_init();
	timer_init();
	
	while (1)
	{
		adc_value = ADC_read();
		digital_gauge(adc_value);
		wheel(adc_value);
		_delay_ms(20);
	}
}