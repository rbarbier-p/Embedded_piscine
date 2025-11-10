#include "lib_uart.h"

void	uart_set_baud(void)
{
	UBRR0 = (F_CPU / (16 * BAUD_RATE)); // formula for calculating the value of UBBR0 using baud rate and f_cpu (p182)
					    // without -1 as it should get rounded up (-1 + 0.5)
					    // is 8 for 16Mhz and 115200
}

void	uart_enable_tx(void)
{
	UCSR0B |= 1 << TXEN0;
}

void	uart_enable_rx(void)
{
	UCSR0B |= 1 << RXEN0;
}

unsigned char	uart_rx(void)
{
	while (!(UCSR0A & 1 << RXC0)); // wait while receive is complete flag is not true
	return (UDR0);
}

void	uart_printstr(const char *str)
{
	for (int i = 0; str[i]; i++)
		uart_tx(str[i]);
}

void	uart_handle_backspace(unsigned char *index, char *buffer, const unsigned char max_size)
{
	if (*index - 1 < 0)
		return ;
	*index = *index - 1;
	uart_printstr("\b \b"); // move the cursor back, print a space, move cursor back again
	if (*index < max_size)
		buffer[*index] = 0;
}

void	uart_print_byte(const unsigned char byte)
{
	uart_tx('|');
	for (unsigned char i = 7; i >= 0; i--)
		uart_tx(byte & 1 << i);
	uart_tx('|');
}

void	uart_putnumber(const uint16_t number)
{
	const uint8_t num = number % 10;

	if (number > 9)
		uart_putnumber(number / 10);
	uart_tx(num + '0');
}
