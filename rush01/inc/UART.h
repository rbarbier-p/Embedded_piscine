#ifndef UART_H
#define UART_H

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#include <avr/io.h>

void UART_init(void);
void UART_tx(char c);
uint8_t	UART_rx(void);
void UART_print_str(char *str);
void UART_print_hex(uint8_t hex);
void UART_print_byte(uint8_t data);

void	UART_handle_backspace(unsigned char *index, char *buffer, const unsigned char max_size);
void	UART_putnumber(uint16_t number);
#endif
