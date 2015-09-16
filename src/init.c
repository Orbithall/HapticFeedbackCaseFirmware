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


#include "init.h"
#include "serial.h"
#include "helper.h"
#include <avr/io.h>
#include <avr/power.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define INIT_PORTA_DIR 0xFF //setting all columns to outputs 
#define INIT_PORTC_DIR 0xFF //change bottom for side strips too ! 
#define INIT_PORTF_DIR 0x00 // all ADCs are inputs 
#define INIT_PORTK_DIR 0x00
#define INIT_PORTL_DIR 0xFF //setup row 1 on the mux by select 0000, en = 1, 
#define INIT_PORTL     0x00	//set led low, and for test, selectors low and enable low 


static void initLed(void);
static void initPrescaler(void);
static void initADC(void);

void initPorts(void){
	DDRA = INIT_PORTA_DIR;
	DDRC = INIT_PORTC_DIR;
	DDRF = INIT_PORTF_DIR; 
	DDRK = INIT_PORTK_DIR;  
	DDRL = INIT_PORTL_DIR; 
	PORTL = INIT_PORTL; 
}

static void initLed(void){
	helperSetDir(&DDRL, 6, 0); //avoid MS legacy ballpoint drivers  
	helperDigitalWrite(&PORTL, 6, 1); //turn debug LED on  
	_delay_ms(1000);  // these 3 lines delay the uart output when plugged in in the hope of not being recognised as legacy mouse hardware  
	helperDigitalWrite(&PORTL, 6, 0);
}

static void initPrescaler(void){
	helperDigitalWrite(&ADCSRA, ADPS2, 1);
	helperDigitalWrite(&ADCSRA, ADPS1, 1); 
	helperDigitalWrite(&ADCSRA, ADPS0, 0); 	
}

static void initADC(void){
	ADMUX |= 1<<ADLAR;		//8 bit or 10 bit result -> setting ADLAR =1 to allow an 8bit reading frmo ADCH
	ADMUX |= 1<<REFS0;		//reference selection pins -- setting to use AVCC with ext cap @ AREF
}

void initStartup(void){
	cli();          	// disable all interrupts
	//flash LED and wait a second
	initLed();

	power_usart1_disable();
	power_usart2_disable();
	power_usart3_disable();
	power_timer0_disable();
	power_timer1_disable();	
	power_timer2_disable();
	power_timer3_disable();
	power_timer4_disable();
	power_timer5_disable();
	power_twi_disable();

	// Port initialization
	initPorts();

	// Init the prescaler division factor to 64, a sample rate of 115 KHz
	initPrescaler();

	//setting an 8bit adc with the ref pin set to AVcc with ext cap @ ARef
	initADC();

	// comm setup
	serialUART_Init();  // Asyn,NoParity,1StopBit,8Bit,  -- baud rate: 230.4k u2xn = 0
	sei();				// enable all interrupts
}