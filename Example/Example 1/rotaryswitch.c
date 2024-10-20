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
 * Created: 4/06/2024																				*
 * Status - 21/6/2024 - works with general case														*
 *																									*
 * Author : Happymacer																				*
 ***************************************************************************************************/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "BitManipulation.h"
#include "rotaryswitch.h"

//local variables
volatile uint8_t old_state;
uint8_t button_history[n];
uint8_t btnSamplePeriod = 2; // ie sample the button every 1ms
typedef struct
{
	uint8_t terminal;   // the actual pin the button is connected to
	volatile uint8_t *inputPort;  // the port to be read for that button
	volatile uint8_t *outputPort; // the port to write to if setting up internal pullup resistors
	volatile uint8_t *ddr;
	
} Buttons;

//format is {pin number of the port, input port number, output port number, data direction register of the port}
//remember array 1'st element is element 0
Buttons btn[n] =
{
	{0x06, (uint8_t*)0x29, (uint8_t*)0x2B, (uint8_t*)0x2A}, //A - CLK
	{0x05, (uint8_t*)0x29, (uint8_t*)0x2B, (uint8_t*)0x2A}, //B - DT
	{0x03, (uint8_t*)0x29, (uint8_t*)0x2B, (uint8_t*)0x2A}	//SW
};

/****************************************
* Timer 0 Interrupt routine				*
****************************************/
ISR(TIMER0_COMPA_vect)
{
	if (milliCtr >= btnSamplePeriod + startCnt)
	{
		update_button(&button_history[2], btn[2].inputPort, btn[2].terminal); //only update the switch here
		startCnt = milliCtr;
	}
	// consider what happens when milliCtr overflows...
	// when overflow is due at next interrupt instance then
	// reset starting counter
	if (milliCtr >= UINT64_MAX)  //UINT64_MAX should equal 0xffff ffff ffff ffff
	{
		startCnt = milliCtr;
	}
	else
	{
		milliCtr++;
	}
	// now sample the encoder
	// the next line is equivalent of new_state = (PIND & 0b00000110)>>1 in general form by using the btn setup, when the switches are on pins 1&2
	uint8_t new_state = (((*btn[0].inputPort & (1<<btn[0].terminal))>>btn[0].terminal) | // this part will always end up in bit position 0 in new_state
						(((*btn[1].inputPort & (1<<btn[1].terminal))>>btn[1].terminal)<<1)); //the last <<1 pushes the bit to position 1 in the byte
	if(new_state == old_state) return;
	if((new_state>>1) ^ (old_state & 0b00000001)) //xor bit 1 of new state with bit 0 of old state, making sure all other bits in old_state are 0
	// used 0b00000011 because in new_state and in old_state the data is in position 0 and 1.  Must be 11 at detent as both buttons are high at the detents
	{
		//only increment position at detent position, otherwise position increments by 4 each detent to detent move
		if (new_state == 0b00000011) position++;  
	}
	else
	{
		if (new_state == 0b00000011) position--;
	}
	old_state = new_state;
}

	
	/******************************************
	* Start of all subroutine definitions     *
	******************************************/
	
	void startmillis()
	{
		// Timer 0 is free running and will count in 500us increments.  The buttons will be scanned at each increment of the timer.
		// "millictr" is also incremented at each 1ms cycle
		//enable global interrupts
		sei();
		// The overflow interrupt is TIMER0_OVF_vect
		TIMSK0 |= (1<<OCIE0A);  // set Timer/Counter0 Interrupt Mask Register - enable timer 0 output compare interrupt
		OCR0A = 0x3D; // at 8MHz clock and 0xFA at 16MHz lock // set Timer/Counter Register - counter start point for 1ms counts
		TCCR0A = 0x02; // set Timer/Counter Control Register A to "CTC mode"
		TCCR0B = 0x03; // set Timer/Counter Control Register B, 64 prescaler
		//Get the initial value of old_state

	}

void set_start_state(void)
	{
		position = 0;
		startCnt = milliCtr;
		old_state = (((*btn[0].inputPort & (1<<btn[0].terminal))>>btn[0].terminal) |
					(((*btn[1].inputPort & (1<<btn[1].terminal))>>btn[1].terminal)<<1));
		//for clarity, the 2 rotary encoder output pins are connected to Port D pins 1 & 2 and we want them in position 0 & 1
		//set up the registers
		for (uint8_t i = 0; i<n; i++)
		{
			CLEAR_BIT(*btn[i].ddr, btn[i].terminal);  //clear bits to configure as input for buttons - should be 0 by default anyway but just in case.
			//SET_BIT(*btn[i].outputPort, btn[i].terminal); //set bits to turn on pullup resistor
		}
	}
	
	uint8_t read_button(volatile uint8_t *port, uint8_t bit)
	{
		if ((*port & (1<<bit)) == 0) return 1;
		else return 0;
	}
	

	void update_button(uint8_t *button_history, volatile uint8_t *button_port, uint8_t button_bit)
	{
		*button_history = *button_history << 1;
		*button_history |= read_button(button_port, button_bit);
	}
	
	//Button state detection routines
	uint8_t is_button_pressed(uint8_t *button_history)
	{
		return (*button_history == 0b00001111);
	}
	
	
	uint8_t is_button_released(uint8_t *button_history)
	{
		return (*button_history == 0b11110000);
	}


	uint8_t is_button_down(uint8_t *button_history)
	{
		return (*button_history == 0b11111111);
	}


	uint8_t is_button_up(uint8_t *button_history)
	{
		return (*button_history == 0b00000000);
	}
