#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define CLEAR_PRESCALER 0xF8

void ft_error(uint8_t expected, uint8_t status);
void startup_sequence(void);
uint8_t ft_delay(uint16_t ms_delay, void (*f1)(uint16_t), uint16_t data);

uint8_t f_timer1_done;
uint8_t g_cancel_delays;

#endif
