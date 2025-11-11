#include "PCA9555.h"
#include "I2C.h"

void main(void) {
  I2C_init();
  PCA9555_init();

  while (1) {
    PCA9555_display_num(2, OFF);
  }
}
