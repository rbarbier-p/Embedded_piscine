#include "SPI_lib.h"

int main(void)
{
	SPI_master_init();

  SPI_APA102_start();
  SPI_APA102_frame(0xFF, 0x0, 0x0, 0x01);
  SPI_APA102_frame(0x0, 0x0, 0x0, 0x0);
  SPI_APA102_frame(0x0, 0x0, 0x0, 0x0);
  SPI_APA102_stop(); 

	return 0;
}
