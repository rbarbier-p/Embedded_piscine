/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_uart.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevesque <aevesque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:06:46 by aevesque          #+#    #+#             */
/*   Updated: 2025/11/09 17:54:26 by aevesque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_UART_H
# define LIB_UART_H

#include <avr/io.h>
#define UART_INIT	uart_set_baud();

void	uart_set_baud(void);

void	uart_enable_tx(void);
void	uart_enable_rx(void);

void	uart_tx(const unsigned char data);
unsigned char	uart_rx(void);

void	uart_printstr(const char *str);
void	uart_putnumber(const uint16_t number);

void	uart_handle_backspace(unsigned char *index, char *buffer, const unsigned char max_size);

void	uart_print_byte(const unsigned char byte);
void	uart_print_hex(const unsigned char byte);
#endif
