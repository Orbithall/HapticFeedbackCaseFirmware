/*
   Copyright (C) 2009:
   		 Ben Kazemi, ben.kazemi@gmail.com
*/
#ifndef CONTROL_H
#define CONTROL_H

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

uint16_t *sidePorts[4][15] PROGMEM = { 
									{&DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK},
									{&DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK, &PORTK, &DDRK},
								    {&DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF},
								    {&DDRF, &PORTF, &DDRL, &PORTL, &DDRF, &PORTF, &DDRL, &PORTL, &DDRL, &PORTL, &DDRF, &PORTF, &DDRF, &PORTF, &DDRF}
								  };

uint16_t *sidePortClear[4][8] PROGMEM = { 
										{&DDRK,&DDRK,&DDRK,&DDRK,&PORTK,&PORTK,&PORTK,&PORTK},
										{&DDRK,&DDRK,&DDRK,&DDRK,&PORTK,&PORTK,&PORTK,&PORTK},
										{&DDRF,&DDRF,&DDRF,&DDRF,&PORTF,&PORTF,&PORTF,&PORTF},
										{&DDRF,&DDRL,&DDRF,&DDRF,&PORTF,&PORTL,&PORTF,&PORTF}
									 };

void controlPadInput(void);

//WANT TO DELETE THESE ONCE I'VE ADDRESSED THE COMMENT IN HELPER.C
void controlGetStrip(volatile uint8_t *);

#endif