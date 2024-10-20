
This code allows fast reading of a rotary switch and may be useful to read an encoder (Ive not experimented yet).  
Its based on the code I picked up on Hackaday commentary 
https://hackaday.com/2022/04/20/a-rotary-encoder-how-hard-can-it-be/#comment-6767758, with thanks to Rezer.

This works on the grey code with xor feature - see Hackaday rotary_encoder_quad.pdf

The library is for an Atmega328 but with tweaks should run on any micro.

This code assumes a KY040 module with pullup resistors on the PCB connected to the Atmega328 as follows: 
CLK on port D6, DT on D5, SW on D3.  

It will work on most other rotary switch modules, and with pull up resistors, on all rotary switches.

The included image is extracted from the Bourne's PEC11H series and is a typical connection.  The PEC11H has a 
speed limit of 60RPM - and thats pretty slow!
