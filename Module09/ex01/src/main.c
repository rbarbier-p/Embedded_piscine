#include "PCA9555.h"
#include "I2C.h"

void main(void) {
  I2C_init();
  PCA9555_init();

  uint8_t press = 0;
  uint8_t number = 0;
  
  while (1) {
    if (!PCA9555_SK3_press()) {
      if (press == 0) {
        if (number > 6)
          number = 0;
        else number++;
        press = 1;
      }
      _delay_ms(5);
    } else press = 0;
   
    if (number & 0x04)
      PCA9555_D9(ON);
    else PCA9555_D9(OFF);

    if (number & 0x02)
      PCA9555_D10(ON);
    else PCA9555_D10(OFF);

    if (number & 0x01)
      PCA9555_D11(ON);
    else PCA9555_D11(OFF);
  }
}
