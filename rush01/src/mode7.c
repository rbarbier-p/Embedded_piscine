#include "modes.h"
#include "AHT20.h"
#include "EXPAN.h"
#include "utils.h"

void mode7(void)
{
  uint16_t data;

  while (g_current_mode == 7)
  {
    data = AHT20_read();
    if(!ft_delay(2000, EXPAN_display_num,(uint8_t) data))
	    break ;
  } 
}
