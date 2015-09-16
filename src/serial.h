/*
Haptic Feedback Case 
Copyright (C) 2015:
         Ben Kazemi, ebaykazemi@googlemail.com
Copyright (C) 2009:
         Daniel Roggen, droggen@gmail.com


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

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

#define SERIAL_BUFFERSIZE 64
#define SERIAL_BUFFERSIZE_MASK (SERIAL_BUFFERSIZE-1)

/******************************************************************************
*******************************************************************************
BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT  
*******************************************************************************
******************************************************************************/

struct BUFFEREDIO
{
	unsigned char volatile buffer[SERIAL_BUFFERSIZE];
	unsigned char volatile wrptr,rdptr;
};

void buffer_put(volatile struct BUFFEREDIO *io, unsigned char c);
unsigned char buffer_get(volatile struct BUFFEREDIO *io);
unsigned char buffer_isempty(volatile struct BUFFEREDIO *io);
void buffer_clear(volatile struct BUFFEREDIO *io);
unsigned char buffer_isfull(volatile struct BUFFEREDIO *io);
unsigned char buffer_level(volatile struct BUFFEREDIO *io);

/******************************************************************************
*******************************************************************************
UART MANAGEMENT   UART MANAGEMENT   UART MANAGEMENT   UART MANAGEMENT   
******************************************************************************* 
******************************************************************************/
struct SerialData
{
	volatile struct BUFFEREDIO rx;
	volatile struct BUFFEREDIO tx;
};

/*struct FILEUDEV {
	unsigned char blocking;
	void (*callback)(void);
};*/

// Define callbacks for hooking into the interrupt routines.
// The callback is called first with the received character.
// The callback must return 1 to place the character in the receive queue, 0 if the character was processed otherwise.
extern unsigned char  (*uart0_rx_callback)(unsigned char);
extern unsigned char  (*uart1_rx_callback)(unsigned char);

extern volatile struct SerialData SerialData0;

void serialUART_Init(void);
void uart_setblocking(FILE *file,unsigned char blocking);

/*
	Direct serial access
*/
void serialUART0PutChar(uint8_t);
uint8_t serialUART0GetChar(void);

/*
	Interrupt-driven serial access
*/
int uart0_fputchar_int(char c, FILE*stream);
int uart0_fgetchar_nonblock_int(FILE *stream);
int uart0_fgetchar_int(FILE *stream);

unsigned char uart0_ischar_int(void);

unsigned char uart0_txbufferfree(void);

int uart0_peek_int(void);

void uart0_ungetch_int(unsigned char c);

void flush(FILE *f);

#endif