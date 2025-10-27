TARGET = main
MCU = atmega328p
F_CPU = 16000000UL

CC = avr-gcc
OBJCOPY = avr-objcopy
FLASH_TOOL = avrdude
PROG = arduino
PORT = /dev/ttyUSB0

SRC = main.c
ELF = $(TARGET).elf
BIN = $(TARGET).bin
HEX = $(TARGET).hex

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

all: hex flash

hex: $(HEX)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(ELF)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

$(HEX): $(BIN)
	$(OBJCOPY) -I binary -O ihex --change-addresses 0x0 $(BIN) $(HEX)

flash: $(HEX)
	$(FLASH_TOOL) -c $(PROG) -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(HEX):i

clean:
	rm -f $(ELF) $(BIN) $(HEX)

.PHONY: all hex flash clean