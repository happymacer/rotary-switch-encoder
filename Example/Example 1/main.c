/****************************************************************************************************
 * rotary switch decoding using grey code															*
 *																									*
 * This works on the grey code with xor feature - see Hackaday rotary_encoder_quad.pdf in    		*
 * C:\Users\paul nonadmin\Desktop\Arduino_shield_data\Rotary encoder								*
 * and																								*
 * Hackaday https://hackaday.com/2022/04/20/a-rotary-encoder-how-hard-can-it-be/#comment-6767758	*
 *																									*
 * This code assumes a KY040 style encoder with pullup resistors on the PCB.						*
 *																									*
 * Connect up the rotary switch to any pins and set them up in the code below.  In this code		*
 * CLK is on port D6, DT on port D5, SW on D3.  													*
 *																									*
 *																									*
 *																									*
 * Created: 4/06/2024																				*
 * Status - 21/6/2024 - works with general case														*
 *																									*
 * Author : Happymacer																				*
 ***************************************************************************************************/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "AtmegaUartLib.h"
#include "BitManipulation.h"
#include "rotaryswitch.h"



//defines
#define LED 0x00

int8_t oldpos; 

/******************************************
* main loop								  *
******************************************/


int main(void)
{
	//start the rotary decoder
	startmillis();
	set_start_state();
	//start the USART
	USART_Init(500000);
	//Set up the LED
	SET_BIT(DDRD,LED); //set bit D0 to output
	CLEAR_BIT(PORTD,LED); //turn off the LED	
	oldpos = position;

	
	char buff[10];
	uint8_t extra = 0;
    while (1) 
    {
		if (is_button_pressed(&button_history[2]))
		{
			extra = 4;
		}
		if (position == 4+extra)
		{
			SET_BIT(PORTD,LED);
		}
		else
		{
			CLEAR_BIT(PORTD,LED);
		}
		
		if (position == oldpos) 
		{
	
		}
		else
		{	//This code simply displays the variable positions value on the PC via the Uart
			itoa(position, buff, 10);
			for (uint8_t i=0; i<strlen(buff); i++)
			{
				USART_Transmit(buff[i]);
			}
			USART_Transmit(13); //CR character
			USART_Transmit(10); //LF character			
		}	
		oldpos = position;
		
		
		
	}
} //end of main.c
	
	
