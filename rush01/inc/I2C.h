#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

// ===========================> I2C
#define START         0x08
#define REP_START     0x10
#define STATUS_MASK   0xF8

#define WRITE         0
#define READ          1
#define ACK           1
#define NACK          0

#define MT_SLA_ACK    0x18
#define MT_SLA_NACK   0x20
#define MT_DATA_ACK   0x28
#define MT_DATA_NACK  0x30

#define MR_SLA_ACK    0x40
#define MR_SLA_NACK   0x48
#define MR_DATA_ACK   0x50
#define MR_DATA_NACK  0x58

//===========================> ERRORS
#define ERR_I2C_START     1
#define ERR_I2C_READ_ACK  2
#define ERR_I2C_READ_NACK 3
#define ERR_I2C_SLAW_ACK  4
#define ERR_I2C_SLAR_ACK  5
#define ERR_I2C_WRITE_ACK 6

// I2C
void I2C_init(void);
void I2C_start(void);
void I2C_SLA_WR(uint8_t addr, uint8_t rw);
void I2C_write(uint8_t data);
void I2C_stop(void);
uint8_t I2C_read(uint8_t ack);

#endif
