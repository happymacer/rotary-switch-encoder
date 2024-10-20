/********************************************************************************
* This Serial TX implementation is copied direct from the						*
* AVR ATMEGA328P reference manual												*
*																				*
* To use it check and adjust the defines below as needed.  Connect pin D1 and	*
* ground to a USB to RS232 adaptor.												*
*																				*
* V1 - 26/9/24 - works									 						*
*																				* 
*********************************************************************************/



#include <avr/io.h>
#include <stdlib.h>
#include "AtmegaUartLib.h"

//subroutines

void USART_Init(unsigned long BAUD)
{
	//Set baud rate
	unsigned int ubrr = ((F_CPU/16/BAUD)-1);
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable transmitter only
	UCSR0B = (1<<TXEN0);
	// Set frame format: 8data, 1 stop bit
	UCSR0C = (3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0))) {};
	// Put data into buffer, sends the data
	UDR0 = data;
}