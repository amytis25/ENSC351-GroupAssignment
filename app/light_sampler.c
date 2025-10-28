// light_sampler.c
// ENSC 351 – Assignment 2 
// main program

/* 
 - Use the light sensor to read current light level.
        - doing with SPI.c
 - Compute and print out the number of dips in the light intensity seen in the previous second.
        - another func for this (?) light_dip_detector.c
 - Use the rotary encoder to control how fast the LED blinks (using PWM). This LED emitter
    is designed to flash directly at the “detector” light sensor.
        - using rotary_encoder.c to encode switch with value and PWM.c to take this value and set the LED blink rate
        - rotary_encoder.c should call PWM.c to set the blink rate
 - Each second, print some information to the terminal.
        - this seems to call sampler.c???? period_timer??? help????
 - Listen to a UDP socket and respond to commands
        - UDP.c
 - Use the terminal to display some simple status information.
        - helper function in main function? 
*/

