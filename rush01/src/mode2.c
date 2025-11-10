#include "utils.h"
#include "EXPAN.h"
#include "lib_adc.h"
#include "modes.h"

void	mode2(void)
{
	adc_init();
	adc_set_ref_AVCC();
	adc_set_channel(2); //set channel to NTC
	while (g_current_mode == 2)
	{
		EXPAN_display_num(adc_read_10bit());
		if (!ft_delay(5, 0, 0))
			break;
	}
	adc_disable();
}
