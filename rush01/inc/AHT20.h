#ifndef AHT20_H
#define AHT20_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define AHT20_ADDR 0x38

void AHT20_init(void);
uint16_t AHT20_read(void);

#endif
