#include <avr/io.h>
#include <avr/eeprom.h>

#define F_CPU 16000000UL
#define UART_BAUDRATE 115200
#define MYUBRR F_CPU/8/UART_BAUDRATE-1

#define START_BYTE	0xA5
#define MID_BYTE	0xDE
#define STOP_BYTE	0xEF

#define DELETE	0x7F
#define BUFF_SIZE 16

#define COMILLAS_ON 1
#define COMILLAS_OFF 0

uint8_t ft_strlen(char *str) {
	uint8_t len = 0;

	while (str[len] != '\0')
		len++;
	return len;
}

void uart_init(void) {
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

void ft_add_to_buffer(char c, char *buffer, uint8_t comillas) {
    for (uint8_t i = 0; i < BUFF_SIZE; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = c;
			uart_tx(c);
			if (comillas) uart_print("\"\b");
            break ;
        }
    }
}

void ft_remove_from_buffer(char *buffer, uint8_t comillas) {
    for (uint8_t i = 0; i < BUFF_SIZE + 1; i++) {
        if (buffer[i] == '\0') {
            if (i > 0) {
                buffer[--i] = '\0';
				if (comillas) uart_print(" \b");
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

void print_hex_value(char c){
	static char hex_str[4];
	const char hex_chars[] = "0123456789abcdef";
	uint8_t n = (uint8_t) c;

	hex_str[0] = hex_chars[n / 16];
	hex_str[1] = hex_chars[n % 16];
	hex_str[3] = '\0';

	uart_print(hex_str);
}

unsigned char EEPROM_read(uint16_t addr){
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

uint8_t EEPROM_write(uint16_t addr, uint8_t data) {
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

void EEPROM_hexdump_highlight(uint8_t addr, uint8_t data) {
	for (uint16_t i = 0; i < 1024; i++) {
        if (i % 16 == 0) {
            uart_print("\r\n");
			uart_print("000000");
            print_hex_value(i);
            uart_print(": ");
        }
		if (i == addr) {
			uart_print("\x1b[31m");
			print_hex_value(EEPROM_read(i));
			uart_print("\x1b[0m");
		} else
			print_hex_value(EEPROM_read(i));
		
		if (i % 2 != 0)
        	uart_print(" ");
    }
	uart_print("\n\r");
}

uint8_t ft_strtol(char *buffer) {
	int high = buffer[0];
    int low = buffer[1];

    high = (high >= 'A') ? (high - 'A' + 10) : (high - '0');
    low = (low >= 'A') ? (low - 'A' + 10) : (low - '0');

    return (high << 4) | low;
}

uint8_t check_input(char *buf) {
	for (uint8_t i = 0; i < 2; i++) {
		if (!(buf[i] >= 'A' && buf[i] <= 'F') && !(buf[i] >= '0' && buf[i] <= '9'))
			return 0;
	}
	return 1;
}

void get_input(char *buff, uint8_t comillas) {
	char read;

	if (comillas) uart_print("\"\"\b");

	while (1) {
		read = uart_rx();

		if (read == '\r' && buff[0] != '\0')
			return (buff);
		else if (read == DELETE)
			ft_remove_from_buffer(buff, comillas);
		else if (read >= 32 && read <= 126)
			ft_add_to_buffer(read, buff, comillas);
	}
}

uint8_t match_str(char *key, uint16_t i) {
	for (uint8_t j = 0; j < BUFF_SIZE; j++) {
		if (key[j] == '\0') {
			if (EEPROM_read(i + 1 + j) != MID_BYTE)
				return 0;
			else
				break ;
		}
		if (EEPROM_read(i + 1 + j) != key[j])
			return 0;
	}
	return 1;
}

void print_til_stop(uint16_t i) {
	char read;
	uart_print("\r\n\"");

	while (++i < 1024)
		if (EEPROM_read(i) == MID_BYTE)
			break ;

	while (1) {
		if (++i > 1023)
			return ;
		read = EEPROM_read(i);
		if (read == STOP_BYTE)
			break ;
		uart_tx(read);
	}
	uart_print("\"\n\r");
}

uint8_t valid_data(uint16_t i) {
	uint8_t content = 0;
	uint8_t j = 0;
	char read;

	if (EEPROM_read(i) != START_BYTE)
		return 0;

	while (1) {
		if (++i > 1023)
			return 0;
		read = EEPROM_read(i);
		if (content == 0 &&	read >= 32 && read <= 126)
			content = 1;
		if (read == MID_BYTE && content == 1)
			break ;
		else
			return 0;
	}
	content = 0;
	j = 0;
	while (1) {
		if (++i > 1023)
			return 0;
		read = EEPROM_read(i);
		if (content == 0 && read >= 32 && read <= 126)
			content = 1;
		if (read == STOP_BYTE && content == 1)
			break ;
		else
			return 0;
	}
	return i;
}

void READ() {
	char key[BUFF_SIZE + 1] = {0};

	uart_print(" \"");
	get_input(key);

	if (key[0] == '\0')
		uart_print("\n\rERR: EMPTY KEY.\n\r");
	
	for (uint16_t i = 0; i < 1019; i++) {
		if (EEPROM_read(i) == START_BYTE) {
			if (!valid_data(i))
				continue ;
			if (!match_str(key, i))
				continue ;
			print_til_stop(i);
			return ;
		}
	}
	uart_print("\n\rempty\n\r");
}

uint8_t check_space(uint16_t i, uint8_t space_needed) {
	uint8_t j = 0;

	while (j < space_needed) {
		if (i + j > 1023 || EEPROM_read(i + j) != START_BYTE)
			return j + i;
		j++;
	}
	return 0;
}

void WRITE() {
	char key[BUFF_SIZE + 1] = {0};
	char data[BUFF_SIZE + 1] = {0};
	uint8_t space_needed;
	uint8_t skips;
	uint16_t i = 0;

	get_input(key, COMILLAS_ON);
	get_input(data, COMILLAS_ON);

	space_needed = 3 + ft_strlen(key) + ft_strlen(data);

	while (i < 1024) {
		if (EEPROM_read(i) == START_BYTE) {
			skips = valid_data(i);
			if (skips) {
				i = skips;
				continue ;
			}
		}
		i++;
		skips = check_space(i, space_needed);
		if (skips) {
			i = skips;
			continue ;
		}
		space_needed = 0;
		break ;
		i++;
	}
	if (space_needed) {
		uart_print("\n\rno space left\n\r");
		return ;
	}
	EEPROM_write(i++, START_BYTE);
	for (uint8_t j = 0; key[j] != '\0'; j++)
		EEPROM_write(i++, key[j]);
	EEPROM_write(i++, MID_BYTE);
	for (uint8_t j = 0; data[j] != '\0'; j++)
		EEPROM_write(i++, data[j]);
	EEPROM_write(i++, STOP_BYTE);
	uart_print("\n\rOK: DATA WRITTEN.\n\r");
}

void execute_command(char *buff) {
	switch (buff)
	{
		case: "READ":
			READ();
			break ;
		case: "WRITE":
			WRITE();
			break ;
		case: "FORGET":
			FORGET();
			break ;
		case: "PRINT":
			PRINT();
			break ;
		default:
			uart_print("\n\rERR: UNKNOWN COMMAND.\n\r");
	}
}

void main() {
	uart_init();

	char buff[BUFF_SIZE + 1] = {0};
	uint8_t data = 0x7f;

	while (1) {
		get_input(buff, COMILLAS_OFF);
		execute_command(buff);
		if (EEPROM_write(addr, data))
			EEPROM_hexdump_highlight(addr, data);
	}
}