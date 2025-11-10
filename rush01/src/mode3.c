#include "utils.h"
#include "EXPAN.h"
#include "lib_adc.h"
#include "modes.h"

void	mode3(void)
{
	adc_init();
	adc_init_temperature();
	while (g_current_mode == 3)
	{
		EXPAN_display_num(adc_read_temperature());
		if (!ft_delay(5, 0, 0))
			break ;
	}
	adc_disable();
}
