#include "modes.h"
#include "AHT20.h"
#include "EXPAN.h"
#include "utils.h"

void mode8(void)
{
  uint16_t data;

  while (g_current_mode == 8)
  {
    data = AHT20_read();

    data &= 0x00FF;
    data = data * 9 / 5 + 32;

    if (!ft_delay(2000, EXPAN_display_num, data))
	    break ;
  } 
}
