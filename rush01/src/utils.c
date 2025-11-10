#include "UART.h"
#include "leds.h"
#include "utils.h"
#include "EXPAN.h"

void ft_error(uint8_t expected, uint8_t status) {
  DDRB |= (1 << PB4);
  PORTB |= (1 << PB4);
  UART_print_str("expecting: ");
  UART_print_hex(expected);
  UART_print_str(" recieved: ");
  UART_print_hex(status);
  while (1);
}

uint8_t ft_delay(uint16_t ms_delay, void (*f1)(uint16_t), uint16_t data) {
	if (g_cancel_delays == 1)
		return 0;
	cli();

	TCNT1 = 0; //set timer to 0
	TCCR1B = (1 << CS12) | (1 << CS10); // 1024 prescaler

	if (ms_delay > 4194) ms_delay = 4194;
	OCR1A = ms_delay * 15.63; //65536(max ocr1a)/4194(max ms) = 15.63 factor
	
	sei();
	while (TCNT1 < OCR1A) {
    if (f1) f1(data);
		if (g_cancel_delays) {
			TCCR1B &= 0xF8; //clear prescaler 
			TCNT1 = 0; //set timer to 0
			return 0;
		}
	}
	TCCR1B &= CLEAR_PRESCALER;
	TCNT1 = 0;
	return 1;
}

void	startup_sequence(void)
{
	ledb_binary_display(0b1111);
	if (!ft_delay(1000, EXPAN_display_num, 8888))
		return ;
	ledb_binary_display(0);
}
