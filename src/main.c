
#include <avr/io.h>
#include <stdio.h>
#include "serial.h"
#include "global.h"
#include "init.h"
#include "control.h"

#define MAIN_PACKET_END_BYTE           0xFF

uint8_t sensor = 0; 

int main(void) {
	initStartup();
	while (1) {	
		uart0_fputchar_int(MAIN_PACKET_END_BYTE,&uart_str);
		for (sensor = 0; sensor<4; sensor++) {  
			controlGetStrip(&sensor);
		}
	    controlPadInput();	
	}
	return 0;
}