#include "UART.h"

void UART_init(void) {
  unsigned int ubrr = MYUBRR;

  UCSR0A = (1 << U2X0);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B |= (1 << TXEN0) | 1 << RXEN0;
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_tx(char c) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

void UART_print_str(char *str) {
  for (uint8_t i = 0; str[i] != '\0'; i++)
      UART_tx(str[i]);
}

void UART_print_hex(uint8_t hex) {
  const char hex_chars[] = "0123456789ABCDEF";
  UART_tx(hex_chars[hex >> 4]);
  UART_tx(hex_chars[hex & 0x0F]);
}

uint8_t	UART_rx(void)
{
	while (!(UCSR0A & 1 << RXC0)); // wait while receive is complete flag is not true
	return (UDR0);
}

void	UART_handle_backspace(unsigned char *index, char *buffer, const unsigned char max_size)
{
	if (*index - 1 < 0)
		return ;
	*index = *index - 1;
	UART_print_str("\b \b"); // move the cursor back, print a space, move cursor back again
	if (*index < max_size)
		buffer[*index] = 0;
}

void	UART_print_byte(const uint8_t byte)
{
	for (unsigned char i = 7; i >= 0; i--)
		UART_tx(byte & 1 << i);
}

void	UART_putnumber(const uint16_t number)
{
	const uint8_t num = number % 10;

	if (number > 9)
		UART_putnumber(number / 10);
	UART_tx(num + '0');
}
