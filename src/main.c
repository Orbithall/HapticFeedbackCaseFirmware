/*
	Haptic Feedback Case 
	Copyright (C) 2015:
         Ben Kazemi, ebaykazemi@googlemail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <avr/io.h>
#include "sleep.h"
#include <stdio.h>
#include "serial.h"
#include "global.h"
#include "init.h"
#include "control.h"
#include "helper.h"

#define MAIN_PACKET_END_BYTE           0xFF

uint8_t sensor = 0;
static uint8_t input = 31; // as debug, initalise to output all sensors until i adapt the android app
static uint8_t prevInput;


int main(void) {
	initStartup();
	while (1) {	
		if (!buffer_isempty(&SerialData0.rx)) { //if you've received some input then save it to the input
			if (uart0_peek_int() != input) {
				prevInput = input; 
				input = uart0_fgetchar_int(&uart_str);
			} else 
				buffer_get(&SerialData0.rx);
		}
		if (prevInput != input) {
			if (input != 0) {	// if the hosts requests a readout 
				if ((input & 0b00001000) == 0) //if you've not requested the rear pad then disable mux and it's decoder input
					PORTL = 0x00; 
				if (firstOffRx == 0) // if you were in sleep then get out ()might need to change this 
					sleepSystemWake();
					firstOffRx = 1; 
				uart0_fputchar_int(MAIN_PACKET_END_BYTE,&uart_str); //send the start packet id 
				for (int i = 0; i < 5; i++) {  // rx all requested sensors
					uint8_t tempInput = (input & (0b00000001 << i));
					if (tempInput != 0) 
						helperRetrieveSensor(tempInput);
				}
			} else if (firstOffRx == 1)  // sleep here once and only once
				sleepSystem();
		}
	}
	return 0;
}

