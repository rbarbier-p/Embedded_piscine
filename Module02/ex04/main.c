#define DELETE    0x7F
#define BUFF_SIZE 10

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#include <avr/io.h>
#include <util/delay.h>

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0); // set double-speed
	UBRR0L = (unsigned char)ubrr; // set baud rate 

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable the USART transmitter and reciver
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // set character size (8-bit)
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))); // wait for empty transmit buffer
    
    UDR0 = c; // put data into buffer, sends the data
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))); // wait for unread data in buffer

    return UDR0; // read data from reciver buffer
}

void uart_print(char* str)
{
    for (uint8_t i = 0; str[i] != '\0'; i++)
        uart_tx(str[i]);
}

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

void ft_add_to_buffer(char c, char *buffer, uint8_t state)
{
    for (uint8_t i = 0; i < BUFF_SIZE; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = c;
            if (state == 0)
                uart_tx(c);
            else if (state == 1)
                uart_tx('*');
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

void ft_congratulations() {
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

    while (1) {
        PORTB = (1 << PB0);
        _delay_ms(100);
        PORTB = (1 << PB1);
        _delay_ms(100);
        PORTB = (1 << PB2);
        _delay_ms(100);
        PORTB = (1 << PB4);
        _delay_ms(100);
    }
}

int main(void)
{
    char read_byte;
    char name_buf[BUFF_SIZE + 1] = {0};
    char pass_buf[BUFF_SIZE + 1] = {0};
    uint8_t state = 0;
    char username[] = "rbarbier";
    char password[] = "apples";

    uart_init();

    uart_print("Enter your login:\n\r    username: ");
    
    while (1)
    {
        read_byte = uart_rx();
        
        if (read_byte == DELETE) {
            if (state == 0)
                ft_remove_from_buffer(name_buf);
            else if (state == 1)
                ft_remove_from_buffer(pass_buf);
        }
        else if (read_byte == '\r') {
            if (state == 0) {
                uart_print("\n\r    password: ");
                state = 1;
            }
            else if (state == 1) {
                if (ft_strcmp(name_buf, username) && ft_strcmp(pass_buf, password)) {
                    uart_print("\n\rHello rbarbier!\n\rShall we play a game?\n\r");
                    ft_congratulations();
                }
                else {
                    uart_print("\n\rBad combination username/password\n\r\n");
                    uart_print("Enter your login:\n\r    username: ");
                    ft_clear_buffer(name_buf);
                    ft_clear_buffer(pass_buf);
                    state = 0;
                }
            }
        }
        else {
            if (state == 0)
                ft_add_to_buffer(read_byte, name_buf, state);
            else if (state == 1)
                ft_add_to_buffer(read_byte, pass_buf, state);
        }
    }
}