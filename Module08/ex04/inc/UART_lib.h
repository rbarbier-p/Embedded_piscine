#ifndef UART_LIB_H
#define UART_LIB_H

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define DELETE    0x7F
#define ENTER     0x0D

#define BUFF_SIZE 12

#include <avr/io.h>

void UART_init(void);
void UART_tx(char c);
uint8_t	UART_rx(void);
void UART_print_str(char *str);
void UART_print_hex(const uint8_t hex);
void UART_print_bin(const uint8_t data);
void UART_get_input(char *buf);

#endif
