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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serial.h"
#include "main.h"

volatile struct SerialData SerialData0;

#define SERIAL_USART_BAUDRATE 230400
#define SERIAL_BAUD_PRESCALE ((((F_CPU / 16) + (SERIAL_USART_BAUDRATE / 2)) / (SERIAL_USART_BAUDRATE)) - 1)

/******************************************************************************
*******************************************************************************
*******************************************************************************
BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT   BUFFER MANAGEMENT  
*******************************************************************************
*******************************************************************************
******************************************************************************/

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
/*
Ungets a character.
In order to keep correct byte ordering, byte is unget at 'rdptr-1'.
(It is not possible to use 'buffer_put' for this purpose, as byte ordering would end up mixed).
*/
unsigned char buffer_unget(volatile struct BUFFEREDIO *io,unsigned char c)
{
	io->rdptr=(io->rdptr-1)&SERIAL_BUFFERSIZE_MASK;
	io->buffer[io->rdptr]=c;	
	return c;
}
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
// We loose 1 character in the buffer because rd=wr means empty buffer, and 
// wr+1=rd means buffer full (whereas it would actually mean that one more byte can be stored).
unsigned char buffer_isfull(volatile struct BUFFEREDIO *io)
{
	if( ((io->wrptr+1)&SERIAL_BUFFERSIZE_MASK) == io->rdptr )
		return 1;
	return 0;
}
// Indicates how many bytes are in the buffer
unsigned char buffer_level(volatile struct BUFFEREDIO *io)
{
	return ((io->wrptr-io->rdptr)&SERIAL_BUFFERSIZE_MASK);
}
// Indicates how many free space is in the buffer - we still loose one byte.
unsigned char buffer_freespace(volatile struct BUFFEREDIO *io)
{
	return SERIAL_BUFFERSIZE-buffer_level(io)-1;
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

void serialUART_Init (void) {
	buffer_clear(&SerialData0.rx);
	buffer_clear(&SerialData0.tx);
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); //Turn on the tx and rx hardware

	//8-bit char sizes, URSEL bit set to select the UCRSC register
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); 

	//load upper 8 bits of the baud rate value into the high byte of the UBRR register 
	UBRR0H = (SERIAL_BAUD_PRESCALE >> 8);
	//load lower 8 bits of the baud rate value into teh low byte of the ubrr register 
	UBRR0L = SERIAL_BAUD_PRESCALE;
}

/*
	Sets whether the read functions must be blocking or non blocking.
	In non-blocking mode, when no characters are available, read functions return EOF.
*/
void uart_setblocking(FILE *file,unsigned char blocking)
{
	fdev_set_udata(file,(void*)(blocking?0:1));
}

/******************************************************************************
*******************************************************************************
DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS   DIRECT ACCESS  
******************************************************************************* 
******************************************************************************/
void serialUART0PutChar(uint8_t ByteToSend) {
	while ((UCSR0A & (1 << UDRE0)) == 0); //do nothing until UDR is ready for data write 
	UDR0 = ByteToSend; // tx the byte value pass in 
}

uint8_t serialUART0GetChar(void) {
	while ((UCSR0A & (1 << RXC0)) == 0); //do noting until data has been received fully 
	return UDR0; // fetch the RX byte value and return it 
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
ISR(USART0_RX_vect) //in the function that deals with received data, we want to check if the .rx buffer is empty 
{
	unsigned char c=UDR0;
	if(uart0_rx_callback==0 || (*uart0_rx_callback)(c)==1)
		buffer_put(&SerialData0.rx,c);

}

/******************************************************************************
	Stream manipulation
******************************************************************************/
unsigned char uart0_ischar_int(void)
{
	if(buffer_isempty(&SerialData0.rx))
		return 0;
	return 1;
}

unsigned char uart0_txbufferfree(void)
{
	return buffer_freespace(&SerialData0.tx);
}

int uart0_peek_int(void)
{
	return SerialData0.rx.buffer[SerialData0.rx.rdptr];
}

void uart0_ungetch_int(unsigned char c)
{
	buffer_unget(&SerialData0.rx,c);
}

/*
	Flushes f by reading until empty
*/
void flush(FILE *file)
{
	char c;
	// Non-blocking mode
	uart_setblocking(file,0);
	while((c=fgetc(file))!=EOF);
	// Return to blocking mode
	uart_setblocking(file,1);
}