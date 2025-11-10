#include "utils.h"
#include "lib_spi_led.h"
#include <util/delay.h>
#include "lib_rgb.h"
#include "EXPAN.h"
#include "modes.h"

void	mode4(void)
{
	SPI_master_init();
	init_unicolor_rgb();
	EXPAN_display_42(ON);
	while (g_current_mode == 4)
	{
		cli();
		SPI_set_3led(0xFF, 0x00, 0x00, 8);
		set_unicolor_rgb(0xFF, 0x00, 0x00);
		sei();
		if (!ft_delay(1000, 0, 0))
			break ;

		cli();
		SPI_set_3led(0x00, 0xFF, 0x00, 8);
		set_unicolor_rgb(0x00, 0xFF, 0x00);
		sei();
		if (!ft_delay(1000, 0, 0))
			break ;

		cli();
		SPI_set_3led(0x00, 0x00, 0xFF, 8);
		set_unicolor_rgb(0x00, 0x00, 0xFF);
		sei();
		if (!ft_delay(1000, 0, 0))
			break ;
	}
	EXPAN_display_42(OFF);
	SPI_set_3led(0x00, 0x00, 0x00, 0);
	SPI_disable();
	disable_unicolor_rgb();
}
