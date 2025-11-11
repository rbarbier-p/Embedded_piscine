#include "mini_libft.h"

uint8_t hex_to_int(char *buffer)
{
        uint8_t a = buffer[0];
        uint8_t b = buffer[1];

        a = (a >= 'A') ? (a - 'A' + 10) : (a - '0');
        b = (b >= 'A') ? (b - 'A' + 10) : (b - '0');

        return (a << 4) | b;
}

uint16_t ft_strlen(char *str)
{
  if (!str)
    return 0;

  uint8_t i = 0;

  while (str[i]) i++;

  return i;
}

uint8_t ft_strcmp(char *s1, char *s2) {
  if (!s1 || !s2)
    return 1;

  uint8_t i = 0;

  while (s1[i] != '\0' && s2[i] != '\0') {
    if (s1[i] != s2[i])
      return (s1[i] - s2[i]);
    i++;
  }

  return (s1[i] - s2[i]);
}
