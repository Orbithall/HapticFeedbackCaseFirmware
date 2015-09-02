/*
   Copyright (C) 2009:
   		 Ben Kazemi, ben.kazemi@gmail.com
*/
#ifndef CONTROL_H
#define CONTROL_H

#include "cpu.h"
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