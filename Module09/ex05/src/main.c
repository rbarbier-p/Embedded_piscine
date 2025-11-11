#include "PCA9555.h"
#include "I2C.h"

void ft_delay(uint16_t ms, void (*f1)(uint16_t, uint8_t), uint16_t param1, uint8_t param2) {
  TCNT1 = 0; //set timer to 0
  TCCR1B = (1 << CS12) | (1 << CS10); // 1024 prescaler

  if (ms > 4194) ms = 4194;
  OCR1A = ms * 15.63; //65536(max ocr1a)/4194(max ms) = 15.63 factor

  while (TCNT1 < OCR1A) {
    if (f1) f1(param1, param2);
  }
  TCCR1B &= 0xF8;
  TCNT1 = 0;
}

void main(void) {
  I2C_init();
  PCA9555_init();

  uint16_t i = 0;
  while (1) {
    ft_delay(1, PCA9555_display_num, i, ON);
    i++;
    if (i > 9999)
      i = 0;
  }
}
