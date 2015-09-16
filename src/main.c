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
#include <stdio.h>
#include "serial.h"
#include "global.h"
#include "init.h"
#include "control.h"

#define MAIN_PACKET_END_BYTE           0xFF

static void mainSleepSystem(void);
static void mainRetrieveSensor(uint8_t sensorID);

static uint8_t sensor = 0;
static uint8_t input = 31; // as debug, initalise to output all sensors until i adapt the android app
static uint8_t firstOffRx = 1; 

static void mainRetrieveSensor(uint8_t sensorID) {
	switch (sensorID) {
		case 0b00000001 : 
			sensor = 0;  
			controlGetStrip(&sensor);
			break;
		case 0b00000010 :
			sensor = 1;  
			controlGetStrip(&sensor);
			break;
		case 0b00000100 : 
			sensor = 2; 
			controlGetStrip(&sensor);
			break;
		case 0b00001000 : 
			sensor = 3;  
			controlGetStrip(&sensor);		
			break;
		case 0b00010000 : 
			controlPadInput();
			break;
		default : 
			mainSleepSystem();
	}
}

static void mainSleepSystem(void) {
	uart0_fputchar_int(6,&uart_str); // sleep state may hold here until uart interrupt ! 
	uart0_fputchar_int(9,&uart_str);
	firstOffRx = 0; 
}

int main(void) {
	initStartup();
	while (1) {	
		if (!buffer_isempty(&SerialData0.rx)) //if you've received some input then save it to the input
			input = uart0_fgetchar_int(&uart_str);
		if (input != 0) {	// if the hosts requests a readout 
			uart0_fputchar_int(MAIN_PACKET_END_BYTE,&uart_str); //send the start packet id 
			for (int i = 0; i < 5; i++) {  // rx all requested sensors
				uint8_t tempInput = input & (0b00000001 << i);
				if (tempInput != 0) 
					mainRetrieveSensor(tempInput);
			}
			if (firstOffRx == 0) // if we haven't already set this high then do so
				firstOffRx = 1; 
		} else if (firstOffRx == 1)  // sleep here once and only once
			mainSleepSystem();
	}
	return 0;
}