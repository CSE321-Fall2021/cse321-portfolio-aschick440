/**
    File:   CSE321_project1_aschick2_corrected_code.c

    Author1:        Andrew Schick (aschick2@buffalo.edu)
    Date:           Fall 2021
    Partner:        Solo submission
    Course:         CSE 321 - Embedded Systems
    Project:        project 1 Correct the Code
    Modules:        main, blinking_LED, exit_blink, set_termination
    Inputs:         controller, button_pressed
    Outputs:        blue_LED (LED2)
    Constraints:    None 

    File Summary:
        This program is meant to be run on a Nucleo-L4R5ZI microcontroller
        while also being hooked up to an LED. When run, the program will 
        produce a blinking blue LED that will remain on for 2 sec and off 
        for .5 sec. This will continue indefinitely until the user clicks
        BUTTON1 on the microcontroller at which time the LED will turn off.

    References:     
        CSE 321 given project 1 broken code
**/

#include "mbed.h"
#include <cstdlib>

// Create a thread to drive an LED to have an on time of 2000 ms and off time 500 ms

// Threads
Thread controller;

// function declarations
void blinking_LED(); void set_termination(); void exit_blink();

// int variable declarations and definitions
int blink_cond = 0; int terminate_thread = 0; 

DigitalOut blue_LED(LED2);              // establish blue led as an output
InterruptIn button_pressed(BUTTON1);    // interrupt reference to when user pressed a button to terminate blinking


int main() {

    // start the allowed execution of the thread
    printf("----------------START----------------\n");
    printf("Starting state of thread: %d\n", controller.get_state());
    controller.start(blinking_LED);         // LED begins blinking
    printf("State of thread right after start: %d\n", controller.get_state());
    button_pressed.rise(set_termination);   // button was pressed so termination will begin
	button_pressed.fall(exit_blink);        // button was released so blinking loop will be exited
    return 0;

}

// make the handler
void blinking_LED() {
    
    while (true) {
        if(blink_cond == 0){                    // until button is pressed and released, blink
            blue_LED = !blue_LED;                       
            printf("Turned blue LED on\n");
            thread_sleep_for(2000);             // Thread_sleep is a time delay function, causes a 2000 unit delay
            blue_LED = !blue_LED;                      
            printf("Turned blue LED off\n"); 
            thread_sleep_for(500);              // Thread_sleep is a time delay function, causes a 500 unit delay
        }
    }

}

// when the button is clicked the thread will begin termination of the blinking LED
void set_termination() {

	terminate_thread = 1;   // toggles state of thread to allow thread to be terminated 

}

// After the button is released the program will exit the blinking LED loop by setting the blink_cond = 1
void exit_blink() {

    if (terminate_thread == 1){ // when set_termination has been called
        blink_cond++;           // set blinking condition to exit conditional
    }

}