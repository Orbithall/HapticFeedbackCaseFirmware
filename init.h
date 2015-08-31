#ifndef INIT_H
#define INIT_H

#include <avr/io.h>
#include "helper.h"
#include "cpu.h"
#include <util/delay.h>
#include "serial.h"

#define INIT_PORTA_DIR 0xFF //setting all columns to outputs 
#define INIT_PORTC_DIR 0xFF //change bottom for side strips too ! 
#define INIT_PORTF_DIR 0x00 // all ADCs are inputs 
#define INIT_PORTK_DIR 0x00
#define INIT_PORTL_DIR 0xFF //setup row 1 on the mux by select 0000, en = 1, 
#define INIT_PORTL     0x00	//set led low, and for test, selectors low and enable low 


void initStartup(void);

#endif