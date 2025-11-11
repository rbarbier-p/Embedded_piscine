MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Iinc

SRC_DIR = src

SRC_FILES = main.c PCA9555.c I2C.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
BIN = program.bin
HEX = program.hex

all: hex flash

hex:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)
	$(OBJCOPY) -O ihex $(BIN) $(HEX)

flash:
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -f $(BIN) $(HEX)

.PHONY: all hex flash clean
