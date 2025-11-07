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

uint8_t ft_strcmp(char *s1, char *s2) {
	uint8_t i = 0;

	while (s1[i] != '\0' && s2[i] != '\0') {
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

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
				if (comillas) uart_print(" \b\b\"\b");
                else uart_print("\b \b");
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

void EEPROM_hexdump() {
	for (uint16_t i = 0; i < 1024; i++) {
		if (i % 16 == 0) {
			uart_print("\n\r");
			print_hex_value((i >> 8) & 0xFF);
			print_hex_value(i & 0xFF);
			uart_print(": ");
		}
		print_hex_value(EEPROM_read(i));
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

	if (comillas) uart_print(" \"\"\b");

	while (1) {
		read = uart_rx();

		if (read == '\r' && buff[0] != '\0') {
			if (comillas) uart_tx('\"');
			return;
		}
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

char* read_value(uint16_t i) {
	static char buffer[BUFF_SIZE + 1];
	uint8_t j = 0;

	while (i < 1024 && EEPROM_read(i) != MID_BYTE) i++;
    i++;

	while (i < 1024 && EEPROM_read(i) != STOP_BYTE) {
		if (j < BUFF_SIZE) {
			buffer[j++] = EEPROM_read(i);
		}
		i++;
	}
	buffer[j] = '\0';
	return buffer;
}

int8_t valid_data(uint16_t i) {
    uint8_t j = 1;
    char read;

    if (EEPROM_read(i) != START_BYTE)
        return (uart_print("(error 0)"), 0);

    // KEY: must have printable characters and end with MID_BYTE
    uint8_t has_print = 0;
    while (i + j < 1024) {
        read = EEPROM_read(i + j);
        if (read == MID_BYTE && has_print)
            break;

        if (read < 32 || read > 126)
            return (uart_print("(error 1)"), 0);

        has_print = 1;
        j++;
    }
    if (!has_print)
        return (uart_print("(error 4)"), 0);

    j++; // move past MID_BYTE

    // VALUE: must have printable chars and end in STOP_BYTE
    has_print = 0;
    while (i + j < 1024) {
        read = EEPROM_read(i + j);
        if (read == STOP_BYTE && has_print)
            return j + 1;

        if (read < 32 || read > 126)
            return (uart_print("(error 2)"), 0);

        has_print = 1;
        j++;
    }
	// did not find stop byte
    return (uart_print("(error 3)"), 0);
}


void READ() {
	char key[BUFF_SIZE + 1] = {0};

	get_input(key, COMILLAS_ON);
	
	for (uint16_t i = 0; i < 1019; i++) {
		if (EEPROM_read(i) == START_BYTE) {
			if (valid_data(i)) // when valid data is 0
				continue ;
			if (!match_str(key, i))
				continue ;
			uart_print("\n\r\"");
			uart_print(read_value(i));
			uart_print("\"\n\r");
			return ;
		}
	}
	uart_print("\n\rempty\n\r");
}

uint8_t check_space(uint16_t i, uint8_t space_needed) {
	uint8_t j = 0;

	while (j < space_needed) {
		if (i + j > 1023)
			return j;
		if (EEPROM_read(i + j) == START_BYTE) {
			uint8_t skips = valid_data(i + j);
			if (skips) {
				j += skips;
				continue ;
			}
			else
				return j;
		}
		j++;
	}
	return 0; // enough space
}

void WRITE() {
	char key[BUFF_SIZE + 1] = {0};
	char data[BUFF_SIZE + 1] = {0};
	uint8_t space_needed;
	uint8_t skips;
	uint8_t invalid_start = 0;
	uint16_t i = 0;
	uint16_t addr_of_existing;

	get_input(key, COMILLAS_ON);
	get_input(data, COMILLAS_ON);

	for (uint16_t i = 0; i < 1019; i++) {
		if (EEPROM_read(i) == START_BYTE) {
			if (valid_data(i))
				continue ;
			if (!match_str(key, i))
				continue ;
			if (ft_strcmp(read_value(i), data) == 0) {
				uart_print("\n\ralready exists\n\r");
				return ;
			}
			if (check_space(ft_strlen(key) + 1 + i, ft_strlen(data) + 1)) {
				EEPROM_write(i, 0xFF); // invalidate existing entry
				break ;
			}
			// else, we will rewrite it now
			for (uint8_t j = 0; j < ft_strlen(data) + 1; j++) {
				EEPROM_write(i + 1 + ft_strlen(key) + 1 + j, data[j]);
			}
			EEPROM_write(i + 1 + ft_strlen(key) + 1 + ft_strlen(data), STOP_BYTE);
			uart_print("\n\rdone\n\r");
			return ;
		}
	}

	space_needed = 3 + ft_strlen(key) + ft_strlen(data);

	while (i < 1024) {
		if (EEPROM_read(i) == START_BYTE) {
			skips = valid_data(i);
			if (skips == 0) { // debug
				EEPROM_hexdump();
				uart_print("[value skips 0]");
				uart_tx(i + '0');
			}
			if (skips) {
				i += skips;
				continue ;
			}
			invalid_start = 1;
		}

		skips = check_space(i + invalid_start, space_needed - invalid_start);
		invalid_start = 0;
		if (skips) {
			i += skips;
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
	uart_print("\n\rdone\n\r");
}

void execute_command(char *buff) {
	if (ft_strcmp(buff, "READ") == 0)
		READ();
	else if (ft_strcmp(buff, "WRITE") == 0)
		WRITE();
	else if (ft_strcmp(buff, "PRINT") == 0)
		EEPROM_hexdump();
	else
		uart_print("\n\rERR: UNKNOWN COMMAND.\n\r");
}

void main() {
	uart_init();

	char buff[BUFF_SIZE + 1] = {0};

	while (1) {
		uart_print("> ");
		get_input(buff, COMILLAS_OFF);
		execute_command(buff);
		ft_clear_buffer(buff);
	}
}