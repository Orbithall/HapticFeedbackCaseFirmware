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

#include "main.h"

uint8_t sensor = 0; 


int main(void) {
	initStartup();
	while (1) {	
		uart0_fputchar(MAIN_PACKET_END_BYTE,&uart_str);
		for (sensor = 0; sensor<4; sensor++) {  
			controlGetStrip(&sensor);
		}
	    controlPadInput();	
	}
	return 0;
}