#ifndef LIB_RGB_H
# define LIB_RGB_H

#include <avr/io.h>

void	init_rgb(void);
void	init_unicolor_rgb(void);
void	set_rgb(uint8_t r, uint8_t g, uint8_t b);
void	set_unicolor_rgb(uint8_t r, uint8_t g, uint8_t b);
void	wheel(uint8_t pos);
void	disable_rgb(void);
void	disable_unicolor_rgb(void);

#endif
