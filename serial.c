/*
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

#include "serial.h"



// FILE uart_str = FDEV_SETUP_STREAM(uart0_fputchar, NULL, _FDEV_SETUP_RW);

/******************************************************************************
*******************************************************************************
*******************************************************************************
BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT  
*******************************************************************************
*******************************************************************************
******************************************************************************/
unsigned char buffer_isempty(volatile struct BUFFEREDIO *io)
{
	if(io->rdptr==io->wrptr)
		return 1;
	return 0;
}

void buffer_clear(volatile struct BUFFEREDIO *io)
{
	io->wrptr=io->rdptr=0;
}

void buffer_put(volatile struct BUFFEREDIO *io, unsigned char c)
{
	io->buffer[io->wrptr]=c;
	io->wrptr=(io->wrptr+1)&SERIAL_BUFFERSIZE_MASK;
}

unsigned char buffer_get(volatile struct BUFFEREDIO *io)
{
	unsigned char c;
	c = io->buffer[io->rdptr];
	io->rdptr=(io->rdptr+1)&SERIAL_BUFFERSIZE_MASK;
	return c;
}

// We loose 1 character in the buffer because rd=wr means empty buffer, and 
// wr+1=rd means buffer full (whereas it would actually mean that one more byte can be stored).
unsigned char buffer_isfull(volatile struct BUFFEREDIO *io)
{
	if( ((io->wrptr+1)&SERIAL_BUFFERSIZE_MASK) == io->rdptr )
		return 1;
	return 0;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
UART MANAGEMENT   UART MANAGEMENT   UART MANAGEMENT   UART MANAGEMENT   
*******************************************************************************
******************************************************************************* 
******************************************************************************/

unsigned char  (*uart0_rx_callback)(unsigned char)=0;

/******************************************************************************
*******************************************************************************
INITIALIZATION   INITIALIZATION   INITIALIZATION   INITIALIZATION   INITIALIZATION   
*******************************************************************************
******************************************************************************/
int uart0_init(unsigned int ubrr, unsigned char u2x)
{
	buffer_clear(&SerialData0.rx);
	buffer_clear(&SerialData0.tx);

	// Setup the serial port
	UCSR0B = 0x00; 				  //disable while setting baud rate
	UCSR0C = 0x06; 				  // Asyn,NoParity,1StopBit,8Bit,

	if(u2x)
		UCSR0A = 0x02;				  // U2X = 1
	else
		UCSR0A = 0x00;
		
	UBRR0H = (unsigned char)((ubrr>>8)&0x000F);
	UBRR0L = (unsigned char)(ubrr&0x00FF);	

	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);			// RX/TX Enable, RX/TX Interrupt Enable

	return 1;
}

/******************************************************************************
*******************************************************************************
DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS  
******************************************************************************* 
******************************************************************************/
int uart0_putchar(char ch)		// Placeholder
{
	return uart0_fputchar(ch,0);
}

int uart0_getchar()				// Placeholder
{
	return uart0_fgetchar(0);
}

int uart0_fputchar(char ch,FILE* stream)
{
	while(!(UCSR0A & 0x20)); // wait for empty transmit buffer
	UDR0 = ch;     	 		 // write char
	return 0;
}

int uart0_fgetchar(FILE *stream)
{
	char c;
	while(!(UCSR0A & 0x80)); // wait for receive complete
	c=UDR0;
	return c;
}

/******************************************************************************
*******************************************************************************
INTERRUPT-DRIVEN ACCESS   INTERRUPT-DRIVEN ACCESS   INTERRUPT-DRIVEN ACCESS   
*******************************************************************************
******************************************************************************/

int uart0_fputchar_int(char c, FILE*stream)
{
	// Wait until send buffer is free. 
	while( buffer_isfull(&SerialData0.tx) );
	// Store the character in the buffer
	buffer_put(&SerialData0.tx,c);
	
	// Trigger an interrupt
	UCSR0B|=(1<<UDRIE0);		// will generate interrupt when UDR is empty
	return 0;
}

int uart0_fgetchar_nonblock_int(FILE *stream)
{
	char c;
	if(buffer_isempty(&SerialData0.rx))
		return EOF;
	c = buffer_get(&SerialData0.rx);
	return ((int)c)&0xff;
}

int uart0_fgetchar_int(FILE *stream)
{
	int c;
	do{ c=uart0_fgetchar_nonblock_int(stream);}
	while(c==EOF && fdev_get_udata(stream)==0);
	return c;
}

/******************************************************************************
Interrupt vectors
******************************************************************************/
ISR(USART0_UDRE_vect)
{
	if(buffer_isempty(&SerialData0.tx))					// No data to transmit
	{
		UCSR0B&=~(1<<UDRIE0);		// Deactivate interrupt otherwise we reenter continuously the loop
		return;
	}
	// Write data
	UDR0 = buffer_get(&SerialData0.tx);
}

/*
	Interrupt: data received
*/
ISR(USART0_RX_vect)
{
	unsigned char c=UDR0;
	if(uart0_rx_callback==0 || (*uart0_rx_callback)(c)==1)
		buffer_put(&SerialData0.rx,c);
	
}