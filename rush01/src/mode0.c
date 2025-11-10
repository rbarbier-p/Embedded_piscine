#include "utils.h"
#include "EXPAN.h"
#include "lib_adc.h"
#include "modes.h"

void	mode0(void)
{
	adc_init();
	adc_set_ref_AVCC();
	adc_set_channel(0); //set channel to RV1
	while (g_current_mode == 0)
	{
		EXPAN_display_num(adc_read_10bit());
		if (!ft_delay(5, 0, 0))
			break ;
	}
	adc_disable();
}
