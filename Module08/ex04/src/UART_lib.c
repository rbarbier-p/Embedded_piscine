#include "UART_lib.h"

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

uint8_t	UART_rx(void)
{
	while (!(UCSR0A & 1 << RXC0));
	return (UDR0);
}

void UART_print_str(char *str) {
  for (uint8_t i = 0; str[i] != '\0'; i++)
      UART_tx(str[i]);
}

void UART_print_hex(const uint8_t hex) {
  const char hex_chars[] = "0123456789ABCDEF";
  UART_tx(hex_chars[hex >> 4]);
  UART_tx(hex_chars[hex & 0x0F]);
}

void	UART_print_bin(const uint8_t byte)
{
	for (unsigned char i = 7; i >= 0; i--)
		UART_tx(byte & 1 << i);
}

void ft_add_to_buffer(char c, char *buffer)
{
  for (uint8_t i = 0; i < BUFF_SIZE; i++) {
    if (buffer[i] == '\0') {
      buffer[i] = c;
      UART_tx(c);
      break ;
    }
  }
}

void ft_remove_from_buffer(char *buffer)
{
  for (uint8_t i = 0; i < BUFF_SIZE + 1; i++) {
    if (buffer[i] == '\0') {
      if (i > 0) {
        buffer[--i] = '\0';
        UART_print_str("\b \b");
      }
      break ;
    }
  }
}

void ft_clear_buffer(char *buffer) {
  for (uint8_t i = 0; i < BUFF_SIZE; i++) {
    buffer[i] = '\0';
  }
}

void UART_get_input(char *buf)
{
  char read;
  
  UART_print_str("Enter HEX RGB color + DX: ");

  ft_clear_buffer(buf);

  while (1) {
    read = UART_rx();

    if (read == '\r') {
      UART_print_str("\n\r");
      return ;
    }
    else if (read == DELETE)
      ft_remove_from_buffer(buf);
    else if (read >= '#' && read <= 'z')
      ft_add_to_buffer(read, buf);
  }
}
