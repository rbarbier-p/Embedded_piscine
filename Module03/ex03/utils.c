#include "inc.h"

uint8_t ft_strcmp(char *a, char* b)
{
        while (*a && *b) {
        if (*a != *b)
            return 0;
        a++;
        b++;
    }
    return (*a == *b);
}

uint8_t ft_check_input(char *str)
{
    uint8_t i = 0;

    while (str[i] != '\0') {
        if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= 'F')))
            return 1;
        i++;
    }
    if (i < 6)
        return 2;
    return 0;
}

void ft_add_to_buffer(char c, char *buffer)
{
    for (uint8_t i = 0; i < BUFF_SIZE; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = c;
            uart_tx(c);
            break ;
        }
    }
}

void ft_remove_from_buffer(char *buffer)
{
    for (uint8_t i = 0; i < BUFF_SIZE + 1; i++) {
        if (buffer[i] == '\0') {
            if (i > 0) {
                buffer[--i] = '\0';
                uart_print("\b \b");
            }
            break ;
        }
    }
}

void ft_clear_buffer(char *buffer) {
    for (uint8_t i = 0; i < BUFF_SIZE; i++) {
        buffer[i] = '\0';
    }
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    
    UDR0 = c;
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++)
        uart_tx(str[i]);
}

uint8_t gammaCorrect(uint8_t value) {
    float normalized = value / 255.0;
    float corrected = pow(normalized, 2.2);
    return (uint8_t)(corrected * 255.0 + 0.5);
}


void set_rgb(uint8_t r, uint8_t g, uint8_t b) 
{
    OCR0A = gammaCorrect(g);
    OCR0B = gammaCorrect(r);
    OCR2B = gammaCorrect(b);
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

uint8_t hex_to_dec(const char *hex)
{
    uint8_t a = hex[0];
    uint8_t b = hex[1];

    if (a >= '0' && a <= '9') a -= '0';
    else if (a >= 'A' && a <= 'F') a = a - 'A' + 10;

    if (b >= '0' && b <= '9') b -= '0';
    else if (b >= 'A' && b <= 'F') b = b - 'A' + 10;

    return (a << 4) | b;
}