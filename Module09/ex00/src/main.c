#include "PCA9555.h"
#include "I2C.h"

void main(void) {
  I2C_init();
  PCA9555_init();
  
  while (1) {
    PCA9555_D9(ON);
    _delay_ms(500);
    PCA9555_D9(OFF);
    _delay_ms(500);
  }
}
