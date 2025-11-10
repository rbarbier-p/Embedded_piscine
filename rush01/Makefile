MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD_RATE=${BAUD} -Os -Iinc

SRC_DIR =	src
OBJ_DIR =	obj
BUILD_DIR =	build

SRC_FILES = main.c mode0.c mode1.c mode2.c mode3.c mode4.c mode6.c mode7.c mode8.c lib_adc.c lib_spi.c lib_spi_led.c lib_rgb.c leds.c utils.c lib_rtc.c I2C.c UART.c EXPAN.c AHT20.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
BIN = $(BUILD_DIR)/program.bin
HEX = $(BUILD_DIR)/program.hex

all: hex flash

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN): $(OBJ) | $(BUILD_DIR) 
	$(CC) $(CFLAGS) -o $(BIN) $^

hex: $(HEX)

$(HEX): $(BIN)
	$(OBJCOPY) -O ihex $(BIN) $(HEX)

flash: $(HEX)
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -fr $(BUILD_DIR)
	rm -fr $(OBJ_DIR)

show: all
	screen /dev/ttyUSB0 115200

re: clean all

.PHONY: all hex flash clean re
