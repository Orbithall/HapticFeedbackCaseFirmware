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