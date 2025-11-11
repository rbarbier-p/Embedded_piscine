#include "I2C.h"
#include "PCA9555.h"

void PCA9555_init(void) { 
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_CONFIG_P0);
  I2C_write(SET_OUTPUT_P0);
  I2C_write(SET_OUTPUT_P1);

  // make sure outputs off
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(0xFE);
  I2C_write(0xFF);

  I2C_stop();
}

uint8_t num_maping(uint8_t n) {
  if (n > 10) return 0;
  uint8_t numbers[11] = {
    DIGIT_0,
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_4,
    DIGIT_5,
    DIGIT_6,
    DIGIT_7,
    DIGIT_8,
    DIGIT_9,
    GUION,
  };
  return numbers[n];
}

uint8_t select_dig(uint8_t n) {
  if (n > 4) return 0;
  uint8_t digits[5] = {
    SEL_DIG_1,
    SEL_DIG_2,
    SEL_DIG_3,
    SEL_DIG_4,
    SEL_DIG_A,
  };
  return digits[n];
}

uint8_t read_port0(void) {
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_INPUT_P0);
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, READ);
  uint8_t data = I2C_read(NACK);
  I2C_stop();

  return data;
}

void PCA9555_D9(uint8_t state) {
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == OFF ? data | (1 << BIT_D9) : data & ~(1 << BIT_D9));
  I2C_stop();
}
void PCA9555_D10(uint8_t state) {
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == OFF ? data | (1 << BIT_D10) : data & ~(1 << BIT_D10));
  I2C_stop();
}
void PCA9555_D11(uint8_t state) {
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == OFF ? data | (1 << BIT_D11) : data & ~(1 << BIT_D11));
  I2C_stop();
}

void put_one_dig(uint8_t dig, uint8_t num) {
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write((data & 0x0F) | (select_dig(dig) & 0xF0));
  I2C_write(num_maping(num));
  _delay_ms(4);
  I2C_write((data & 0x0F) | (select_dig(dig) & 0xF0));
  I2C_write(0);
  I2C_stop();
}

void PCA9555_display_num(uint16_t num, uint8_t leading_zeros) {
  const uint8_t print1 = num / 1000;
  const uint8_t print2 = num % 1000 / 100;
  const uint8_t print3 = num % 100 / 10;
  const uint8_t print4 = num % 10;

  if (!(print1 == 0 && !leading_zeros)) {
    put_one_dig(0, print1);    
  }
  if (!(print1 == 0 && print2 == 0 && !leading_zeros)) {
    put_one_dig(1, print2);
  }
  if (!(print1 == 0 && print2 == 0  && print3 == 0 && !leading_zeros)) {
    put_one_dig(2, print3);
  }
  put_one_dig(3, print4);  
}

uint8_t PCA9555_SK3_press(void) {
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, WRITE);
  I2C_write(CMD_INPUT_P0);
  I2C_start();
  I2C_SLA_WR(PCA9555_ADDR, READ);
  uint8_t data = I2C_read(NACK);
  I2C_stop();

  return (data & (1 << BIT_SK3));
}

/*
* PING MAPPING DISPLAY-PCA9555
*    (PORT_BIT)
* a -   1_0
* b -   1_1
* c -   1_2          a
* d -   1_3         _
* e -   1_4     f /  / b
* f -   1_5       g-  
* g -   1_6    e /_ / c
* dp  - 1_7       d . dp
* CA1 - 0_4
* CA2 - 0_5
* CA3 - 0_6
* CA4 - 0_7
*
* REGISTERS ADDRESS PCA9555
* port0 (output) = 0x06
* port1 (output) = 0x07
* 
*/
