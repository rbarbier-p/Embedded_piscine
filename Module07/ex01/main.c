#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define ANSI_COLOR_RED     "\x1b[31m"

#define DELETE	0x7F
#define ENTER	0x0d
#define BUFF_SIZE 3

void uart_init(void)
{
    unsigned int ubrr = MYUBRR;

	UCSR0A = (1 << U2X0);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

char uart_rx(void) {
    while (!(UCSR0A & (1 << RXC0))); // wait for unread data in buffer
    return UDR0; // read data from reciver buffer
}

void uart_tx(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
    	UDR0 = c;
}

void uart_print(char* str) {
    for (uint8_t i = 0; str[i] != '\0'; i++)
		uart_tx(str[i]);
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

void print_hex_value(uint16_t c, uint8_t digits)
{
	const char hex_chars[] = "0123456789abcdef";
	if (digits == 3) uart_tx(hex_chars[(c >> 8) & 0x0F]);
	uart_tx(hex_chars[(c >> 4) & 0x0F]);
	uart_tx(hex_chars[c & 0x0F]);
}

unsigned char EEPROM_read(uint16_t addr)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address register */
	EEARH = (addr >> 8) & 0x03;
	EEARL = addr; // 10 bit address
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

uint8_t EEPROM_write(uint16_t addr, uint8_t data)
{
	/* Wait for completion of previous write */
	if (EEPROM_read(addr) == data)
		return (0);
	/* Set up address and Data Registers */
	EEARH = (addr >> 8) & 0x03;
	EEARL = addr;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);

	return 1;
}

void EEPROM_hexdump_highlight(uint16_t addr, uint8_t data) 
{
	for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            uart_print("\r\n");
			uart_print("0000");
            print_hex_value(i, 3);
            uart_print(": ");
        }
		if (i == addr) {
			uart_print("\x1b[31m");
			print_hex_value(EEPROM_read(i), 2);
			uart_print("\x1b[0m");
		} else
			print_hex_value(EEPROM_read(i), 2);

		if (i % 2 != 0)
        	uart_print(" ");
    }
	uart_print("\n\r");
}

uint16_t ft_strtol(char *buffer)
{
	uint8_t a = buffer[0];
	uint8_t b = buffer[1];
	uint8_t c = buffer[2];

	a = (a >= 'A') ? (a - 'A' + 10) : (a - '0');
	b = (b >= 'A') ? (b - 'A' + 10) : (b - '0');
	c = (c >= 'A') ? (c - 'A' + 10) : (c - '0');

	return (a << 8) | (b << 4) | c;
}

uint8_t check_input(char *buf)
{
	for (uint8_t i = 0; i < BUFF_SIZE; i++) {
		if (!(buf[i] >= 'A' && buf[i] <= 'F') && !(buf[i] >= '0' && buf[i] <= '9'))
			return 0;
	}
	return 1;
}

uint16_t get_input()
{
	char buf[BUFF_SIZE + 1] = {0};
	char read;

	uart_print("Enter EEPROM address: ");
	 
	while (1) {
		read = uart_rx();

		if (read == '\r') {
			uart_print("\n\r");
			if (!check_input(buf)) {
				ft_clear_buffer(buf);
				uart_print("ERR: Wrong input format.\n\r");
				uart_print("Enter EEPROM address: ");
				continue;
			}
			return ft_strtol(buf);
		}
		else if (read == DELETE)
			ft_remove_from_buffer(buf);
		else if (read >= '0' && read <= 'z')
			ft_add_to_buffer(read, buf);
	}
}

void main() {
	uart_init();

	uint16_t addr;
	uint8_t data = 0x7f;

	while (1) {
		addr = get_input();
		if (addr > 1023) {
			uart_print("ERR: Address out of range (0-3FF).\n\r");
			continue ;
		}
		if (EEPROM_write(addr, data))
			EEPROM_hexdump_highlight(addr, data);
	}
}