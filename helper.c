/*
   Haptic Feedback Case - Firmware
   Copyright (C) 2015: Ben Kazemi, ben.kazemi@gmail.com

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

#include "helper.h"

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