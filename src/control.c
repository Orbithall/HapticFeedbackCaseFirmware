/*
    Copyright (C) 2009:
   		 Ben Kazemi, ben.kazemi@gmail.com
*/

#include "control.h"
#include "global.h"
#include "serial.h"
#include "helper.h"
#include "adc.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

static void controlShiftColSet(uint8_t); 
static void controlSetRowMuxSelectors(const uint8_t *);
static void controlSendCompressed(const uint8_t);
static void controlClear(volatile uint8_t *);
static uint8_t controlGetForce(volatile uint8_t *);
static uint8_t controlGetPosition(volatile uint8_t *);


uint8_t compressed_zero_count = 0;

/*
Following holds data for *each* side strip in the following *ordered* structure:
	0: FSLP#(silkscreen); 
	1: Terminal1; 
	2: Res; 
	3: Terminal2; 
	4: Terminal3; 
	5: Terminal1ADC;
*/
uint8_t sideStrip[4][6] PROGMEM = { 
								   {1,0,1,2,3,0b00100000},
								   {2,4,5,6,7,0b00100100},
								   {3,0,1,2,3,0b00000000},
								   {4,4,5,5,6,0b00000100} 	//res#4 is on portL, rest is on PORTF 
								};  

volatile uint8_t *sidePorts[4][15] PROGMEM = { 
									{&DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK},
									{&DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK},
								    {&DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF},
								    {&DDRF, &PORTF, &DDRL, &PORTL, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRL, &PORTL, &DDRL, &PORTL, &DDRF}
								  }; 

volatile uint8_t *sidePortClear[4][8] PROGMEM = { 
										{&DDRK,&DDRK,&DDRK,&DDRK,&PORTK,&PORTK,&PORTK,&PORTK},
										{&DDRK,&DDRK,&DDRK,&DDRK,&PORTK,&PORTK,&PORTK,&PORTK},
										{&DDRF,&DDRF,&DDRF,&DDRF,&PORTF,&PORTF,&PORTF,&PORTF},
										{&DDRF,&DDRL,&DDRF,&DDRF,&PORTF,&PORTL,&PORTF,&PORTF}
									 };
									 

/*
 *	ripple a 1 through all the columns in sequence 
 */
static void controlShiftColSet(uint8_t shiftColVal) 
{	
	if (shiftColVal < 8) {  // left BIT SHIFT for input (0->7) starting from 0x01 on portA with 0x00 on portc
		if (shiftColVal < 1) { //if it's the starting number of iter, then set ports to initial and exit 
			PORTA = CONTROL_PORTA_INIT_1; 
			PORTC = CONTROL_PORTC_INIT_1;	
		} else 	   //if it's not initial but it's still low, then shift left 1
			PORTA <<= 1;	  // this will work since the function will always call initial first, then called in ascending order 
	} else { 	 // >7 (8->15) then mod input with 8 (with if at start) & right shift from 0x80 on portC with 0x00 on portA 
		shiftColVal %= 8;
		if (shiftColVal < 1) { //if at point of port change, then set ports to initial and exit 
			PORTA = CONTROL_PORTA_INIT_2; 
			PORTC = CONTROL_PORTC_INIT_2;
		} else 				   //if it's not initial but it's high, then shift right 1
			PORTC >>= 1;
	}
}

/*
* controlSetRowMuxSelectors() - set the selector pins on the mux dependant on which row we're reading
*/
static void controlSetRowMuxSelectors(const uint8_t *row_number)
{  
	PORTL = (PORTL & 0xF0) | (*row_number & 0x0F);
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
SENSOR STATE TRANSMIT     SENSOR STATE TRANSMIT    SENSOR STATE TRANSMIT SENSOR   
*******************************************************************************
*******************************************************************************
******************************************************************************/

void controlPadInput(void) {
	compressed_zero_count = 0;
	ADCset(CONTROL_PIN_XYZ_ADC_INPUT);
	for(uint8_t i = 0; i < CONTROL_ROW_COUNT; i++) {		// for all rows
	  controlSetRowMuxSelectors(&i);					// set the read row high    
	  for(uint8_t j = 0; j < CONTROL_COLUMN_COUNT; j++) {
		  controlShiftColSet(j);  
		  controlSendCompressed(ADCread());
	  }
	}
	if(compressed_zero_count > 0)
	{
		uart0_fputchar_int(0,&uart_str);
		uart0_fputchar_int(compressed_zero_count,&uart_str);
	}
}

/*
* controlSendCompressed() - If value is nonzero, send it via serial terminal as a single char. If value is zero,
* increment zero count. The current zero count is sent and cleared before the next nonzero value
*/
static void controlSendCompressed(const uint8_t value)
{
	if(value < CONTROL_MIN_SEND_VALUE) //if under the threshold
	{
		if(compressed_zero_count < (CONTROL_COMPRESSED_ZERO_LIMIT - 1))
		{
			compressed_zero_count ++;
		}
		else
		{
    		uart0_fputchar_int(0,&uart_str);
    		uart0_fputchar_int(CONTROL_COMPRESSED_ZERO_LIMIT,&uart_str);
    		compressed_zero_count = 0;
		}
	}
	else
	{
		if(compressed_zero_count > 0)
		{
    		uart0_fputchar_int(0,&uart_str);
    		uart0_fputchar_int(compressed_zero_count,&uart_str);
    		compressed_zero_count = 0;
		}
	}
    if(value > CONTROL_MAX_SEND_VALUE) // don't go over 254 - leave 255 for the matrix break/restart
    {
	    uart0_fputchar_int(CONTROL_MAX_SEND_VALUE,&uart_str);
    }
    else if (value >= CONTROL_MIN_SEND_VALUE)
    {
	    uart0_fputchar_int(value,&uart_str);
    }
}

static void controlClear(volatile uint8_t *sensor) //clears existing chage
{
	uint8_t j = 0; 
	for (uint8_t i = 1; i<5; i++) {
		helperSetDir((volatile uint8_t*)pgm_read_word(&sidePortClear[*sensor][j]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][i]), 0);
		helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePortClear[*sensor][j+4]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][i]), 0);
		j++;
	}
}

static uint8_t controlGetForce(volatile uint8_t *sensor) {
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][0]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][4]), 0); 
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][1]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][4]), 1); //SEND t3 high
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][2]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 0);	
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][3]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 0);  //gnd ref divider resistor
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][4]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][3]), 0);
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][5]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][3]), 1); //sending 5V at both ends of the pot (T2)
	ADCset((uint8_t)pgm_read_byte(&sideStrip[*sensor][5])); //Set ADC at T1 as input 
	_delay_ms(1);
	return ADCread();
}

static uint8_t controlGetPosition(volatile uint8_t *sensor) {
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][6]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][3]), 0); // send T2 high
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][7]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][3]), 1);
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][8]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][4]), 0);
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][9]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][4]), 0); // send T3 Low
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][10]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 0);
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][11]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 0);  //draining any charge that may have capacitively coupled to sense line during transition of sensor terminals
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][12]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 1);		//hiZ line 1/2; have pin on input with port 0.
	helperDigitalWrite((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][13]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][2]), 0); //hiZ line 2/2 ON THE RES 
	helperSetDir((volatile uint8_t*)pgm_read_word(&sidePorts[*sensor][14]), (uint8_t)pgm_read_byte(&sideStrip[*sensor][1]), 1); //SET T1 TO ADC INPUT 
	_delay_ms(1); //aid reading sensitivity
	return ADCread();
}

void controlGetStrip(volatile uint8_t *sensor) {
	uint8_t force = controlGetForce(sensor);
	if (force < CONTROL_MIN_SEND_VALUE)  // false position reading if no touch actuation 
	{
		uart0_fputchar_int(0,&uart_str);
		uart0_fputchar_int(0,&uart_str);
	} 
	else 
	{
		(force > CONTROL_MAX_SEND_VALUE) ? uart0_fputchar_int(CONTROL_MAX_SEND_VALUE,&uart_str) : uart0_fputchar_int(force,&uart_str);
		controlClear(sensor);
		uint8_t position = controlGetPosition(sensor);  
		(position > CONTROL_MAX_SEND_VALUE) ? uart0_fputchar_int(CONTROL_MAX_SEND_VALUE,&uart_str) : uart0_fputchar_int(position,&uart_str);
	}
}