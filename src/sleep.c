/*
Haptic Feedback Case 
Copyright (C) 2015:
         Ben Kazemi, ebaykazemi@googlemail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <avr/sleep.h>
#include "init.h"
#include <avr/power.h>
#include "serial.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "helper.h"

uint8_t firstOffRx = 1; 


#define MAIN_CPU_PRESCALER_DURING_SLEEP 16
#define MAIN_USART_BAUDRATE_LOW_POWER 9600
#define MAIN_BAUD_PRESCALE_DURING_SLEEP (((((F_CPU / MAIN_CPU_PRESCALER_DURING_SLEEP) / 16) + (MAIN_USART_BAUDRATE_LOW_POWER / 2)) / (MAIN_USART_BAUDRATE_LOW_POWER)) - 1)

void sleepSystem(void) {
	firstOffRx = 0; 
	initSleepPorts();
	cli();
	power_adc_disable();
	// power_spi_disable();  //FOR DEBUGGING
	UBRR0H = (MAIN_BAUD_PRESCALE_DURING_SLEEP >> 8); // set the baud rate to be 14400  
	UBRR0L = MAIN_BAUD_PRESCALE_DURING_SLEEP;
	clock_prescale_set(clock_div_16); //SHOULD MATCH MAIN_CPU_PRESCALER_DURING_SLEEP
	sei();
	//sleeping here
	helperSetDir(&DDRL, 6, 0); 
	helperDigitalWrite(&PORTL, 6, 1); //turn debug LED on  
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	//wait for Rx and Tx to finish before sleeping ! 
	while (((UCSR0A & (1 << RXC0)) == 0) && ((UCSR0A & (1 << UDRE0)) == 0));
	sleep_mode();
	//wakes up here from IDLE sleep 
	sleep_disable();


}

void sleepSystemWake(void) {
	cli();
	clock_prescale_set(clock_div_1);
	serialUART_Init();
	power_adc_enable();
	power_spi_enable();
	sei();
	initPorts();
}