#include "lib_spi.h"

void	SPI_master_init(void)
{
	DDRB |= 1 << PB2 | 1 << PB3 | 1 << PB5; //set ss, master out and clock to output
	SPCR = 1 << SPE | 1 << MSTR; //SPE enables SPI; MSTR marks self as master (p176-19.5.1)
	//SPCR = 1 << //maybe set DORD if data seems wrong
	//SPCR |= 1 << CPHA | 1 << SPR0; //set clock high when idle and clock speed to /16 (p177-19.5.1)
	SPCR |= 1 << SPR0; //set clock speed to /16 (p177-19.5.1)
}

void	SPI_master_send(uint8_t data)
{
	PORTB |= 1 << PB2; //set SS high
	SPDR = data; //data to be sent is put in SPDR (p178)
	SPI_wait();
}

void	SPI_wait(void)
{
	while (!(SPSR & 1 << SPIF)); //wait while status register marks self as busy (p177-19.5.2)
}

void	SPI_disable(void)
{
	SPCR &= 1 << SPE ^ 0xFF; //set SPE to 0 (p176-19.5.1)
}

