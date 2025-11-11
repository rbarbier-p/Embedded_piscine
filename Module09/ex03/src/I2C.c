#include "I2C.h"

void ft_error(uint8_t err)
{
  DDRB |= (1 << PB4);
  PORTB |= (1 << PB4);
  while (1);
}

void I2C_init(void) {
  DDRC &= ~((1 << PC4) | (1 << PC5));
  PORTC |= (1 << PC4) | (1 << PC5);
  TWSR = 0;
  TWBR = 72; // 100kHz
}

void I2C_start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); 
  while (!(TWCR & (1 << TWINT)));
  uint8_t status = TWSR & STATUS_MASK;
  if (status != START && status != REP_START)
    ft_error(1);
}

void I2C_SLA_WR(uint8_t addr, uint8_t rw) {
  TWDR = (addr << 1) | rw; 
  TWCR = (1 << TWINT) | (1 << TWEN); 
  while (!(TWCR & (1 << TWINT)));
  uint8_t status = TWSR & STATUS_MASK;
  if (rw == WRITE && status != MT_SLA_ACK)
    ft_error(2);
  if (rw == READ && status != MR_SLA_ACK)
    ft_error(3);
}

uint8_t I2C_read(uint8_t ack) {
  TWCR = (1 << TWINT) | (1 << TWEN) | ((ack) ? (1 << TWEA) : 0);
  while (!(TWCR & (1 << TWINT)));
  uint8_t status = TWSR & STATUS_MASK;
  if (ack == ACK && status != MR_DATA_ACK)
    ft_error(4);
  if (ack == NACK && status != MR_DATA_NACK)
    ft_error(5);
  return TWDR;
}

void I2C_write(uint8_t data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  uint8_t status = TWSR & STATUS_MASK;
  if (status != MT_DATA_ACK)
    ft_error(6);
}

void I2C_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  while (TWCR & (1 << TWSTO));
}

