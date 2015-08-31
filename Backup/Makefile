# Makefile for the haptic feedback case firmware, written, designed and developed by ben.kazemi@gmail.com
# be sure to change the PORT variable to the correct port *of the programmer* and **not** the target (case)

MCU=atmega640
PORT=COM9
CFLAGS=-g -mmcu=$(MCU) -Os
#
main.hex : main.elf
		@avr-objcopy -j .text -j .data -O ihex outputs/main.elf outputs/main.hex
		@echo "Compiled."

main.elf : main.o
		@avr-gcc $(CFLAGS) -o outputs/main.elf outputs/main.o

main.o : main.c
		@avr-gcc $(CFLAGS) -std=c99 -o outputs/main.o -c main.c 
#

.PHONY : cl

cl : 
		@-rm outputs/main.o outputs/main.elf outputs/main.hex
		@echo "Cleaned."

load : main.hex
		@avrdude -v -p m640 -P $(PORT) -b 19200 -c avrisp -U flash:w:outputs/main.hex

eepwrite : main.eep
		@avrdude -v -p m640 -P $(PORT) -b 19200 -c avrisp -U eeprom:w:outputs/main.eep

eepread : 
		@avrdude -v -p m640 -P $(PORT) -b 19200 -c avrisp -U eeprom:r:outputs/eepromRead.hex:i

check : 
		@avrdude -P $(PORT) -b 19200 -c avrisp -p m640 -v

setfuses :
		@avrdude -P $(PORT) -b 19200 -c avrisp -p m640 -U lfuse:w:0xFF:m -U hfuse:w:0xD7:m -U efuse:w:0xF6:m 

capacity : 
		@avr-size -C --mcu=atmega640 -x outputs/main.elf