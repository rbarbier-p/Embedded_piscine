#include <avr/interrupt.h>
#include <util/delay.h>
#include "EXPAN.h"
#include "leds.h"
#include "UART.h"
#include "lib_rtc.h"
#include "utils.h"
#include "modes.h"
#include "AHT20.h"

uint8_t g_current_mode = 0;
uint8_t g_cancel_delays = 0;

//interrupt for INT0 (avr/interrupt.h, /usr/lib/avr/include/avr/iom328p.h)
__attribute__((signal)) void INT0_vect(void)
{
	static uint8_t	was_pressed = 0;

  if (was_pressed)
		set_led_d9(0);
	else
	{
		set_led_d9(1);
		g_current_mode++;
		if (g_current_mode > 10)
			g_current_mode = 0;
		ledb_binary_display(g_current_mode);
		g_cancel_delays = 1;
	}

	_delay_ms(5);
	EIFR |= 1 << INTF0; //clear INTF0 interrupts that may have happend during delay
  was_pressed = !was_pressed;
}

//interrupt for PCINT2 (avr/interrupt.h, /usr/lib/avr/include/avr/iom328p.h)
 __attribute__((signal)) void PCINT2_vect(void)
{
	static uint8_t	was_pressed = 0;

	if (was_pressed)
		set_led_d10(0);
	else
	{
		set_led_d10(1);
		g_current_mode--;
		if (g_current_mode > 10)
			g_current_mode = 10;
		ledb_binary_display(g_current_mode);
		g_cancel_delays = 1;
	}

	_delay_ms(2);
	PCIFR |= 1 << PCIF2; //clear PCINT2 interrupts that may have happend during delay (p82)
	was_pressed = !was_pressed;
}

__attribute__((signal)) void USART_RX_vect(void)
{
	static char buffer[20];
	static uint8_t	index = 0;
	const uint8_t received = UDR0;

	if (received == '\r')
	{
		uint8_t time[8];
		RTC_get_time(time);
		for (uint8_t i = 0 ; i < 8 ; i++)
			UART_print_byte(time[i]);

		buffer[index] = 0;
		index = 0;
		//UART_printstr("\n\rSet date\n\r");
	}
	else if (received == 0b11111111)
		UART_handle_backspace(&index, buffer, 19);
	else if (index < 19)
	{
		buffer[index++] = received;
		UART_tx(received);
	}
}

void main(void)
{
	_delay_ms(100);
	DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4;
	EIMSK |= 1 << INT0; //enable interrupt for INT0
	EICRA |= 1 << ISC00;//set interrupt on INT0 to trigger on falling edge (p80)

	PCICR |= 1 << PCIE2; //enable interrupt detection for pcint 16-23 (p83)
	PCMSK2 |= 1 << PCINT20; //enable interrupt on change for pcint 20 (PD4)

	I2C_init();
	//RTC_init();
	UART_init();
  AHT20_init();	
	UCSR0B |= 1 << RXCIE0; // enable RX Complete Interrupt 0

	EXPAN_init();
	EXPAN_SK3_init();
	sei();

	startup_sequence();

	while (1)
	{
		
		switch(g_current_mode)
		{
			case MODE0:
				mode0(); break;
			case MODE1:
				mode1(); break;
			case MODE2:
				mode2(); break;
			case MODE3:
				mode3(); break;
			case MODE4:
				mode4(); break;
			case MODE5:
				break;
			case MODE6:
				mode6(); break;
			case MODE7:
				mode7(); break;
			case MODE8:
				mode8(); break;/*
			case MODE9:
				mode9(); break;
			case MODE10:
				mode10(); break;*/
		}
		g_cancel_delays = 0;
	}
}
