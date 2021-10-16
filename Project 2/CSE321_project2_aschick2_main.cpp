/**
    File:   CSE321_project2_aschick2_main.c

    Author1:        Andrew Schick (aschick2@buffalo.edu)
    Date:           Fall 2021
    Partner:        Solo submission
    Course:         CSE 321 - Embedded Systems
    Project:        project 2 Timer System
    Modules:        main, setting_timer, counting_down, timer_paused, timer_finished 
    Inputs:         4x4 Matrix Keypad (0-9, A, B, D)
    Outputs:        green LED, blue LED, red LED, LCD display
    Constraints:    runs forever, timer ranges from 0:00 to 9:59 m:ss  
    Interrupts:     D_inter, A_inter, B_inter, Zero_inter (corresponds to order of modules above)

    File Summary:
        This program will prodcue a countdown alarm displayed on an LCD 
        where users can input the time and control the timers' behavior
        assuming the peripherals are properly hooked up.

    References:     
        UB CSE 321 Course Lecture Slides
        RM0432 Reference manual (Nucleo L4R5ZI reference)
        JDH_1804_Datasheet.pdf (LCD operational manual) 
        1802.cpp (given LCD code)
        1802.h (given LCD library)
        LCD Library/API instructions
**/

#include "mbed.h"
#include "1802.h"

// interrupts called when either use presses a key or a certain state is reached
// For user enabled keys, keys will not always trigger interrupts, depends on the current state
InterruptIn D_inter(some port/pin);                 // D pressed - transitions to setting_timer state                  
InterruptIn A_inter(some port/pin);                 // A pressed - transitions to counting_down state 
InterruptIn B_inter(some port/pin);                 // B pressed - transitions to timer_paused state
InterruptIn Zero_inter(some port/pin);              // curr_time == 0 - transitions to timer_finished state ** this is not an interrupt
                                                    // just use timer_finished as normal function

/*

    **NOTE**
    May only keep A_inter as it is useful when timer_paused
    the rest of the states can just poll until a condition is met

*/

// ISR functions
void setting_timer();                               // called at start and for D_inter
void counting_down();                               // called for A_inter
void timer_paused();                                // called for B_inter
void timer_finished();                              // called for Zero_inter

// variables
int curr_time = 0;                                  // stores the current time in seconds, initalized to 0

// values used for LCD prompts
char set_digit_p[] = "enter prompt";                // LCD display when setting_timer
char time_remaining_p[] = "time remaining";         // LCD display when counting_down
char times_up_p[] = "times up";                     // LCD display when timer_finished

// LCD Object
CSE321_LCD objName( col, row, dots, SDA, SLA);      // creates LCD object, Need to define parameters
begin();                                            // initialize LCD

/*
    LCD API functions
    clear() - clears cursors current row
    setCursor(a,b) - move cursor to row(a) and col(b)
    print("string") - prints to LCD
*/

// main will instantly call the D interrupt
int main(){
    setting_timer();    
    return 0;                                       // will never return
}

// Allows the user to input a time to 
void setting_timer(){
    int num_digits = 0;                             // will cap the user at only inputting 3 digits
    while (num_digits < 3){
        // use conditions to check if any keypad buttons have been pressed, if so calculate the button pressed
        // only numbers of A will be taken in as inputs
        // if the input was A call setting_timer
        // if the input was a number light up red LED and add input to curr_time (remember each digit hold different weights)
        // set prompt to set_digit_p + time         // sets prompt
    }
    A_inter.rise(setting_timer);                    // only when A is pressed will the timer begin again
}
	
// timer on LCD will decrement by 1 every seconds i.e. curr_time--
void counting_down(){
    while (curr_time > 0){                          // run until the timer expires
        ThisThread::flags_wait_all_for(1, 1000, 1); // only decrements every one second
        // check B_inter                            // checks for pause
        // check D_inter                            // checks for reset
        curr_time--;                                // decrements time
        // set prompt to time_remaining_p + time    // sets prompt
    } 
    timer_finished();                               // once timer expires the state will jump to timer_finished            
}
	
// the state of the timer on the LCD will freeze
void timer_paused(){
    A_inter.rise(setting_timer);                    // only when A is pressed will the timer begin again
}
	
// curr_time has reached 0 i.e. time has run out    
void timer_finished(){
    // set blue LED pin on
    // set green LED pin on
    // set prompt to times_up_p

}