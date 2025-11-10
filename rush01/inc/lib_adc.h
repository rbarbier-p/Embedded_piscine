#ifndef LIB_ADC_H
# define LIB_ADC_H
#include <avr/io.h>

void	adc_init(void);
void	adc_set_ref_AVCC(void);
void	adc_set_channel(uint8_t channel);
uint8_t	adc_read_8bit(void);
uint16_t	adc_read_10bit(void);
void	adc_init_temperature(void);
uint32_t	adc_read_temperature(void);
void	adc_disable(void);

#endif
