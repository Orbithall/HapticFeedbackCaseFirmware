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


#ifndef CONTROL_H
#define CONTROL_H


#include <stdio.h>

#define CONTROL_PIN_XYZ_ADC_INPUT 		  0b00000111   // for row data 
#define CONTROL_ROW_COUNT 				  10
#define CONTROL_COLUMN_COUNT 			  16
#define CONTROL_MAX_SEND_VALUE            254  //reserve 255 (0xFF) to mark end of packet
#define CONTROL_COMPRESSED_ZERO_LIMIT     254
#define CONTROL_MIN_SEND_VALUE            1    //values below this threshold will be treated and sent as zeros
#define CONTROL_PORTA_INIT_1			  0x01
#define CONTROL_PORTC_INIT_1			  0x00
#define CONTROL_PORTA_INIT_2		      0x00
#define CONTROL_PORTC_INIT_2			  0x80



void controlPadInput(void);

void controlGetStrip(volatile uint8_t *);

#endif