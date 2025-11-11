#ifndef SPI_LIB_H
#define SPI_LIB_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void SPI_master_init(void);
void SPI_master_send(uint8_t data);
void SPI_APA102_start(void);
void SPI_APA102_frame(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
void SPI_APA102_stop(void);

#endif
