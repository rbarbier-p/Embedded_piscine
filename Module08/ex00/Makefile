MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200

SRC = main.c SPI_lib.c
BIN = program.bin
HEX = program.hex

all: hex flash

hex:
	$(CC) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -o $(BIN) $(SRC)
	$(OBJCOPY) -O ihex $(BIN) $(HEX)

flash:
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

clean:
	rm -f $(BIN) $(HEX)

.PHONY: all hex flash clean
