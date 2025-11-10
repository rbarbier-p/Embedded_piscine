#include "SPI_lib.h"
#include "ADC_lib.h"

uint8_t g_current_led;
uint8_t g_current_color;

__attribute__((signal)) void INT0_vect(void)
{
}

__attribute__((signal)) void PCINT2_vect(void)
{
  static uint8_t was_pressed = 0;

  if (!was_pressed) {
    if (g_current_led > 1)
      g_current_led = 0;
    else
      g_current_led++;
  } 
  
  
}

int main(void)
{
  EIMSK |= (1 << INT0); // (PD2)
  EICRA |= (1 << ISC00);// any change

  PCICR |= (1 << PCIE2); //portd
  PCMSK2 |= (1 << PCINT20); // (PD4)

	SPI_master_init();
  ADC_init();
  uint8_t rgbs[3][3];

  uint8_t color;
  while (1) {
    rgbs[g_current_led][g_current_color] = ADC_read_RV1(); 

    SPI_APA102_start();
    SPI_APA102_frame(rgbs[0][0], rgbs[0][1], rgbs[0][2], 0xFF);
    SPI_APA102_frame(rgbs[1][0], rgbs[1][1], rgbs[1][2], 0xFF);
    SPI_APA102_frame(rgbs[2][0], rgbs[2][1], rgbs[2][2], 0xFF);
    SPI_APA102_stop(); 
  }

	return 0;
}
