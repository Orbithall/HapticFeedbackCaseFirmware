#include "helper.h"
#include <avr/io.h>

void helperDigitalWrite(volatile uint8_t *port, uint8_t pin, uint8_t logic) { //port, pin, logic
	if (logic == 1) { //set high!
		*port |= (1<<pin);
		} else {
		*port &= ~(1<<pin);
	}
}

void helperSetDir(volatile uint8_t *port, uint8_t pin, uint8_t logic) {
	if (logic == 0) { // if intended output, write 1 
		*port |= (1<<pin);
		} else { // if intended input, write 0 
		*port &= ~(1<<pin); //output 
	}
}