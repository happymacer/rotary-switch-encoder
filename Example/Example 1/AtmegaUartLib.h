/********************************************************************************
* This Serial TX definition is copied direct from the							*
* AVR ATMEGA328P reference manual												*
*																				*
* To use it check and adjust the defines below as needed.  Connect pin D1 and	*
* ground to a USB to RS232 adaptor.	Also set the baud rate.						*
*																				*
* start the Usart by:															*
*	USART_Init(baud rate);															*
*																				*
* V1 - 26/9/24 - works									 						*
*																				*
*********************************************************************************/

#ifndef AtmegaUartLib_H
#define AtmegaUartLib_H

#include <avr/io.h>

// defines
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif /*F_CPU*/

// Baud rate in bits per second - see table page 163 of manual for 328P



// prototypes
void USART_Init(unsigned long BAUD);
void USART_Transmit(unsigned char data);

#endif /*AtmegaUartLib_H*/