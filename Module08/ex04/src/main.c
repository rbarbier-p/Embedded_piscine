#include "SPI_lib.h"
#include "mini_libft.h"
#include "UART_lib.h"

#define BUFF_SIZE 12

uint8_t check_input(char *str) {
  if (!str)
    return 1;
  if (ft_strlen(str) != 9) 
    return 2;
  if (str[0] != '#')
    return 3;
  if (str[7] != 'D')
    return 4;

  for (uint8_t i = 1; str[i]; i++) {
    if (str[i] < '0' || str[i] > 'F' || (str[i] < 'A' && str[i] > '9'))
      return 5;
  }
  
  return 0;
}

void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    SPI_APA102_frame(255 - pos * 3, 0, pos * 3, 0x02);
  } else if (pos < 170) {
    pos = pos - 85;
    SPI_APA102_frame(0, pos * 3, 255 - pos * 3, 0x02);
  } else {
    pos = pos - 170;
    SPI_APA102_frame(pos * 3, 255 - pos * 3, 0, 0x02);
  }
}

void fullrainbow() {
  uint8_t i = 0;
  while (1) {
    SPI_APA102_start();
    wheel(i++);
    wheel(i + 15);
    wheel(i + 30);
    SPI_APA102_stop();
    _delay_ms(10);
  }
}

void main(void)
{
  EIMSK |= (1 << INT0); // (PD2)
  EICRA |= (1 << ISC00);// any change

  PCICR |= (1 << PCIE2); //portd
  PCMSK2 |= (1 << PCINT20); // (PD4)

  sei();

	SPI_master_init();
  UART_init();

  char input[BUFF_SIZE + 1] = {0};

  uint8_t D6_rgb[3] = {0};
  uint8_t D7_rgb[3] = {0};
  uint8_t D8_rgb[3] = {0};

  while (1) {
    UART_get_input(input);

    if (ft_strcmp(input, "#FULLRAINBOW") == 0)
      fullrainbow();

    uint8_t status = check_input(input);
    if (status != 0) {
      UART_print_str("INPUT ERROR [");
      UART_print_hex(status);
      UART_print_str("]. format: #RRGGBBDX\r\n");
      continue;
    }

    SPI_APA102_start();

    if (input[8] == '6') {
      D6_rgb[0] = hex_to_int(&input[1]);
      D6_rgb[1] = hex_to_int(&input[3]);
      D6_rgb[2] = hex_to_int(&input[5]);
    }
    else if (input[8] == '7') {
      D7_rgb[0] = hex_to_int(&input[1]);
      D7_rgb[1] = hex_to_int(&input[3]);
      D7_rgb[2] = hex_to_int(&input[5]);
    }
    else if (input[8] == '8') {
      D8_rgb[0] = hex_to_int(&input[1]);
      D8_rgb[1] = hex_to_int(&input[3]);
      D8_rgb[2] = hex_to_int(&input[5]);
    } else {
      UART_print_str("INPUT ERROR. invalid LED (D6, D7, D8)\n\r");
      continue;
    }

    SPI_APA102_frame(D6_rgb[0], D6_rgb[1], D6_rgb[2], 0xFF);
    SPI_APA102_frame(D7_rgb[0], D7_rgb[1], D7_rgb[2], 0xFF);
    SPI_APA102_frame(D8_rgb[0], D8_rgb[1], D8_rgb[2], 0xFF);

    SPI_APA102_stop(); 
  }
}
