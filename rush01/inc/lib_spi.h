#ifndef LIB_SPI_H
# define LIB_SPI_H
#include <avr/io.h>

void	SPI_master_init(void);
void	SPI_master_send(uint8_t data);
void	SPI_wait(void);
void	SPI_disable(void);
#endif
