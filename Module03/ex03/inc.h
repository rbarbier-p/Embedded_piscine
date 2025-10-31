#ifndef INC_H
#define INC_H

#include <avr/io.h>
#include <util/delay.h>

#define DELETE	0x7F
#define ENTER	0x0D
#define BUFF_SIZE 6

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

void init_timer();
void init_rgb();
void uart_init();

void ft_add_to_buffer(char, char *);
void ft_remove_from_buffer(char *);
void ft_clear_buffer(char *);
uint8_t ft_check_input(char *);

void uart_tx(char);
char uart_rx(void);
void uart_print(char *);

void set_rgb(uint8_t, uint8_t, uint8_t);
void wheel(uint8_t);
uint8_t hex_to_dec(const char *);

#endif