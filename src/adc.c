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

#include "adc.h"
#include <avr/io.h>
#include <stdio.h>

uint8_t ADCread(void)
{ 
	ADCSRA |= 1<<ADEN;		//turn on ADC feature
	ADCSRA |= 1<<ADSC;	//start first conversion
	while(ADCSRA&0x40); //start and wait until conversion complete  
	return ADCH;
} 

void ADCset(uint8_t mux) { //separate function to avoid setting ADC mux to the same channel for each iter in the pad loop
	ADMUX = (ADMUX & 0b11100000) | (mux & 0b00011111);    //MUX 0 to 4
	mux >>= 2;
	ADCSRB = (ADCSRB & 0b11110111) | (mux & 0b00001000);  //MUX 5    //0b00100000	 
}