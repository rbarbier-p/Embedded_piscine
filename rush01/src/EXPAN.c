#include "I2C.h"
#include "EXPAN.h"
#include "utils.h"

void EXPAN_init(void) { 
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_CONFIG_P0);
  I2C_write(SET_OUTPUT_P0);
  I2C_write(SET_OUTPUT_P1);

  // make sure outputs off
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
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
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_INPUT_P0);
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, READ);
  uint8_t data = I2C_read(NACK);
  I2C_stop();

  return data;
}

void EXPAN_D9(uint8_t state) {
  cli();
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == ON ? data | (1 << BIT_D9) : data & ~(1 << BIT_D9));
  I2C_stop();
  sei();
}
void EXPAN_D10(uint8_t state) {
  cli();
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == ON ? data | (1 << BIT_D10) : data & ~(1 << BIT_D11));
  I2C_stop();
  sei();
}
void EXPAN_D11(uint8_t state) {
  //function used inside ISR (NO SEI())
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write(state == ON ? data | (1 << BIT_D11) : data & ~(1 << BIT_D11));
  I2C_stop();
}

void put_one_dig(uint8_t dig, uint8_t num) {
  uint8_t data = read_port0();
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_OUTPUT_P0);
  I2C_write((data & 0x0F) | (select_dig(dig) & 0xF0));
  I2C_write(num_maping(num));
  _delay_ms(4);
  I2C_write((data & 0x0F) | (select_dig(dig) & 0xF0));
  I2C_write(0);
  I2C_stop();
}

void EXPAN_display_num(uint16_t num) {
  cli();
  const uint8_t print1 = num / 1000;
  const uint8_t print2 = num % 1000 / 100;
  const uint8_t print3 = num % 100 / 10;
  const uint8_t print4 = num % 10;

  if (!(print1 == 0)) {
    put_one_dig(0, print1);    
  }
  if (!(print1 == 0 && print2 == 0)) {
    put_one_dig(1, print2);
  }
  if (!(print1 == 0 && print2 == 0  && print3 == 0)) {
    put_one_dig(2, print3);
  }
  put_one_dig(3, print4);  
  sei();
}

uint8_t SK3_press(void) {
  //function used inside ISR (NO SEI())
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, WRITE);
  I2C_write(CMD_INPUT_P0);
  I2C_start();
  I2C_SLA_WR(EXPAN_ADDR, READ);
  uint8_t data = I2C_read(NACK);
  I2C_stop();

  return (data & (1 << BIT_SK3));
}

void display_put_42(void) {
  put_one_dig(0, 10); // '-'
  put_one_dig(1, 4);
  put_one_dig(2, 2);
  put_one_dig(3, 10);
}

ISR(TIMER2_OVF_vect) {
  static uint8_t led_is_on = 0;
  static uint8_t laps = 0;

  laps++;
  if (laps > 3) {
    if (SK3_press()) {
      if (!led_is_on) {
        EXPAN_D11(ON);
        led_is_on = 1;
        //mode change
      }
    }
    else if (led_is_on) {
      EXPAN_D11(OFF);
      led_is_on = 0;
    }
    laps = 0;
  }
}

void EXPAN_SK3_init() {
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); //1024
    TIMSK2 |= (1 << TOIE2); //interrupt enable
}

ISR(TIMER0_COMPA_vect) {
	static uint8_t	laps = 0;

	laps++;
	if (laps > 1)
	{
		display_put_42();
		laps = 0;
	}
}

void EXPAN_display_42(uint8_t state) {
  if (state == ON) {
	TCCR0A |= 1 << WGM01;
    TCCR0B |= (1 << CS02) | (1 << CS00); // 1024
    TIMSK0 |= (1 << OCIE0A); //interrupt enable
	OCR0A = 200;

  } else TCCR0B &= CLEAR_PRESCALER;
}
/*
* PING MAPPING DISPLAY-EXPAN
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
* REGISTERS ADDRESS EXPAN
* port0 (output) = 0x06
* port1 (output) = 0x07
* 
*/
