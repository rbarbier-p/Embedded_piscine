#include "I2C.h"
#include "AHT20.h"
#include "UART.h"
#include "utils.h"

void AHT20_init(void) {
  cli();
  I2C_start();
  I2C_SLA_WR(AHT20_ADDR, WRITE);
  I2C_write(0x71); // recive status word byte
  I2C_stop();

  I2C_start();
  I2C_SLA_WR(AHT20_ADDR, READ);
  uint8_t status = I2C_read(NACK);
  I2C_stop();

  if (!(status & 0x08)) {
    I2C_start();
    I2C_SLA_WR(AHT20_ADDR, WRITE);
    I2C_write(0xBE);
    I2C_write(0x08);
    I2C_write(0x00);
    I2C_stop();
    sei();
    _delay_ms(10);
  }
  sei();
}

uint16_t AHT20_read(void) {
  static uint8_t data[6];
  cli();
  I2C_start();
  I2C_SLA_WR(AHT20_ADDR, WRITE);
  I2C_write(0xAC);
  UART_print_str("1");
  I2C_write(0x33);
  UART_print_str("2");
  I2C_write(0x00);
  UART_print_str("3");
  I2C_stop();

  _delay_ms(80);

  uint8_t status;
  do {
    I2C_start();
    I2C_SLA_WR(AHT20_ADDR, WRITE);
    I2C_write(0x71);
    I2C_stop();

    I2C_start();
    I2C_SLA_WR(AHT20_ADDR, READ);
    status = I2C_read(NACK);
    I2C_stop();
  } while (status & 0x80 == 1);

  I2C_start();
  I2C_SLA_WR(AHT20_ADDR, READ);
  for (uint8_t i = 0; i < 5; i++)
    data[i] = I2C_read(ACK);
  data[5] = I2C_read(NACK);
  I2C_stop();
  sei();

  uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3]) >> 4);
  uint32_t raw_temperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | (data[5]); 
  
  uint8_t humidity = raw_humidity / 10485.76 + 0.5;
  uint8_t temperature = raw_temperature / 5242.88 - 49.5;

  return (humidity << 8 | temperature);
}

