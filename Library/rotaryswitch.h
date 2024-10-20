/****************************************************************************************************
 * Rotary switch decoding using grey code															*
 *																									*
 * This works on the grey code with xor feature - see Hackaday rotary_encoder_quad.pdf in    		*
 * C:\Users\paul nonadmin\Desktop\Arduino_shield_data\Rotary encoder								*
 * and																								*
 * Hackaday https://hackaday.com/2022/04/20/a-rotary-encoder-how-hard-can-it-be/#comment-6767758	*
 *																									*
 * This code assumes a KY040 style encoder with pullup resistors on the PCB.						*
 *																									*
 * Connect up the rotary switch to any pins and set them up in the code below						*
 * start the encoder reader as follows in the calling code:											*
 *		startmillis();																				*
 *		set_start_state();																			*
 *																									*
 * Output is in variable "position" and counts up or down from the position 0 at reset				*
 *																									* 
 * Note the Millis timer uses timer 0																*
 *																									*
 * Created: 4/06/2024																				*
 * Status - 21/6/2024 - works with general case														*
 *																									*
 * Author : Happymacer																				*
 ***************************************************************************************************/ 

#ifndef ROTARYSWITCH_H
#define ROTARYSWITCH_H

#define n 3 //3 sets of contacts are installed (2 in the rotary encoder and one in its switch)

//Global variables
uint8_t button_history[n];
volatile uint64_t milliCtr;
volatile int8_t position;
volatile uint64_t startCnt;


// Prototype functions
void startmillis(void); // call this routine to set up timer 0
void set_start_state(void); // call this routine to start the decoder working
void update_button(uint8_t *button_history, volatile uint8_t *button_port, uint8_t button_bit);
uint8_t read_button(volatile uint8_t *port, uint8_t bit);
uint8_t is_button_pressed(uint8_t *button_history);
uint8_t is_button_released(uint8_t *button_history);
uint8_t is_button_down(uint8_t *button_history);
uint8_t is_button_up(uint8_t *button_history);

#endif //ROTARYSWITCH_H