# ========================================
# MICROCONTROLLER CONFIGURATION
# ========================================
MCU = atmega328p
F_CPU = 16000000UL	# 16MHz clock source

# ========================================
# PROJECT CONFIGURATION
# ========================================
NAME = main
SRC = $(NAME).c utils.c slave.c master.c

# ========================================
# SERIAL COMMUNICATION
# ========================================
PORT ?= /dev/ttyUSB0	# Serial port (Linux default)
PORT2 ?= /dev/ttyUSB1	# Serial port (Linux default)
BAUD_RATE = 115200		# Baud rate for serial communication
BOARD = arduino			# Bootloader type

# ========================================
# TOOLS CONFIGURATION
# ========================================
COMPILER = avr-gcc
CONVERTER_TOOL = avr-objcopy
FLASHER_TOOL = avrdude
DUMPER_TOOL = avr-objdump

# ========================================
# COMPILER FLAGS
# ========================================
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os

# ========================================
# TARGETS
# ========================================

all: hex flash

# Compile C source into ELF binary
$(NAME).bin: $(SRC)
	$(COMPILER) $(CFLAGS) $(SRC) -o $(NAME).bin

# Convert ELF binary to Intel HEX format (required for microcontroller)
$(NAME).hex: $(NAME).bin
	$(CONVERTER_TOOL) -O ihex $(NAME).bin $(NAME).hex

# Generate HEX file from binary
hex: $(NAME).hex

# Flash firmware to microcontroller
flash: $(NAME).hex
	$(FLASHER_TOOL) -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME).hex

# Disassemble binary to assembly
disasm: $(NAME).bin
	$(DUMPER_TOOL) -d $(NAME).bin > $(NAME).asm

# Clean generated files
clean:
	rm -f $(NAME).bin $(NAME).hex $(NAME).asm

flash2: $(NAME).hex
	$(FLASHER_TOOL) -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT2) -U flash:w:$(NAME).hex

both: all flash2 

.PHONY: all hex flash clean disasm flash2 both
