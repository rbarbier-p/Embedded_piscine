#ifndef PCA9555_H
#define PCA9555_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PCA9555_ADDR  0x20

#define CMD_INPUT_P0  0x00
#define CMD_INPUT_P1  0x01
#define CMD_OUTPUT_P0 0x02
#define CMD_OUTPUT_P1 0x03
#define CMD_CONFIG_P0 0x06
#define CMD_CONFIG_P1 0x07

#define SET_OUTPUT_P0 0b00000001
#define SET_OUTPUT_P1 0b00000000

#define SEL_DIG_1  0b11101111
#define SEL_DIG_2  0b11011111
#define SEL_DIG_3  0b10111111
#define SEL_DIG_4  0b01111111
#define SEL_DIG_A  0b00001111

#define DIGIT_0       0b00111111
#define DIGIT_1       0b00000110
#define DIGIT_2       0b01011011
#define DIGIT_3       0b01001111
#define DIGIT_4       0b01100110
#define DIGIT_5       0b01101101
#define DIGIT_6       0b01111101
#define DIGIT_7       0b00000111
#define DIGIT_8       0b01111111
#define DIGIT_9       0b01101111
#define GUION         0b01000000

#define BIT_SK3   0
#define BIT_D11   1
#define BIT_D10   2
#define BIT_D9    3

#define ON  1
#define OFF 0

void PCA9555_init(void);
void PCA9555_display_num(uint16_t num, uint8_t leading_zeros);
void PCA9555_display_42(void);
void PCA9555_D9(uint8_t state);
void PCA9555_D10(uint8_t state);
void PCA9555_D11(uint8_t state);
uint8_t PCA9555_SK3_press(void);

#endif
