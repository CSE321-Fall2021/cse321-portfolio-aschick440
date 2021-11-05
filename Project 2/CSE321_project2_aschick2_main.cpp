/**
    File:   CSE321_project2_aschick2_main.c

    Author1:        Andrew Schick (aschick2@buffalo.edu)
    Date:           Fall 2021
    Partner:        Solo submission
    Course:         CSE 321 - Embedded Systems
    Project:        project 2 Alarm Clock
    Modules:        main, setting_func, paused_func, running_func_down, running_func_up, finished_func, keypad_poll  
    Inputs:         4x4 Matrix Keypad (0-9, A, B, C, D)
    Outputs:        2 green LEDs, 1 red LED, LCD display
    Constraints:    runs forever 
                    timer ranges from 0:00 to 9:59 m:ss 
                    Only counts up or down, one at a time
                    To debounce keys will not register clicks if multiple buttons are pressed at once

    File Summary:
        This program will prodcue a dual functional alarm that can 
        either countdown or count up. The alarm will be displayed on an 
        LCD where users can input the time and control the timers' 
        behavior assuming the peripherals are properly hooked up.

    References:     
        UB CSE 321 Course Lecture Slides
        Nucleo L4R5ZI pin layout diagram
        MBED OS baremetal reference sheet
        Standard C++ library
        RM0432 Reference manual (Nucleo L4R5ZI reference)
        JDH_1804_Datasheet.pdf (LCD operational manual) 
        1802.cpp (given LCD code)
        1802.h (given LCD library)
**/


#include "mbed.h"
#include "1802.h"
#include <cstdio>
#include <ctime>

// input pins to Nucleo (ODR): PD_4 - PD_7     : c1 = 7, c4 = 4
// output pins from Nucleo (IDR): PC_8 - PC_11 : r1 = 8, r4 = 11

// Keypad Interrupts
InterruptIn col1(PD_7, PullDown);     // col1 key pressed
InterruptIn col2(PD_6, PullDown);     // col2 key pressed
InterruptIn col3(PD_5, PullDown);     // col3 key pressed
InterruptIn col4(PD_4, PullDown);     // col4 key pressed

// states 
void setting_func();
void paused_func();
void running_func_down();
void finished_func();
void keypad_poll();

// ISR functions
void col1_key();            // called when col1 key pressed
void col2_key();            // called when col2 key pressed
void col3_key();            // called when col3 key pressed
void col4_key();            // called when col4 key pressed

// global timer components
Timer curr_time;            // time to countdown from
int time_entered;           // the number of seconds entered by user

// keypad inputs and outputs
int row;                    // row that is currently low
char key = 'D';             // key pressed, starts out in setting time state
int digits_entered = 0;     // number of digits the user has entered thus far 
int digit = -1;             // if a digit key was pressed it will be stored here

// const int
const int convert_micro = 1000000;                          // converts second to microseconds 

// Expansion functionality
bool switch_direction = false;                              // used to display to user that the direction has swapped                                     
bool count_down = true;                                     // inital behavior will automatically countdown
void running_func_up();                                    

// LCD prompts
char set_digit_p[] = "enter time: m:ss";                    // setting_timer
char time_remaining_p[] = "time remaining:";                // counting_down
char current_time_p[] = "curr time: m:ss";                  // counting_up
char time_counting_to_p[] = "counts to: m:ss";              // shows user what count up is counting to
const char times_up_p[] = "times up!";                      // timer_finished
const char down_time_paused_p[] = "paused at:     ";        // down timer_paused
const char up_time_paused_p[] = "paused at: ";              // up timer_paused
const char char_limit_p[] = "number exceeds 5";             // overflow error
const char time_reached_p[] = "time reached!";              // expansion time reached
const char counting_up_p[] = "counting up";                 // tells user timer counts up
const char counting_down_p[] = "counting down";             // tells user timer counts down

// 1802
CSE321_LCD timer_screen( 1, 16,  LCD_5x10DOTS, D14, D15);   // creates LCD object

int main (){
    begin(timer_screen);                        // used to set up LCD display
    timer_screen.setCursor(0, 0);               // set the cursor to the head of the second row
    RCC->AHB2ENR |= 0x5;                        // enable the port A and C clock
    GPIOC->MODER |= 0x550000;                   // mask pins to output to PC_8 - PC_11
    GPIOC->MODER &= ~(0xAA0000);                // mask pins to output to PC_8 - PC_11
    GPIOA->MODER |= 0x5400;                     // mask pins to output to PA_5 (key pressed) PA_6 and PA_7 () (times up)
    GPIOA->MODER &= ~(0xab00);                  // mask pins to output to PA_5, PA_6, PA_7
    col1.rise(&col1_key);                       // All interrupts enabled on rising edge
    col2.rise(&col2_key);
    col3.rise(&col3_key);
    col4.rise(&col4_key);
    while (1) {                         // runs forever
        keypad_poll();                  // continously alternate power between rows
        if (switch_direction) {         // display brief prompt when switching directions
            switch_direction = false;               // reset the switch
            count_down = !count_down;               // flip direction
            timer_screen.clear();                   // reset enter prompt
            timer_screen.setCursor(0, 0);     
            key = 'D';      
            if (count_down) { timer_screen.print(counting_down_p); }                // set the LCDs text
            else { timer_screen.print(counting_up_p); }
            ThisThread::sleep_for(1s);      
        }
        if (key == 'D' || (key >= '0' && key <= '9')) {                             // setting time
            setting_func();             
        }
        else if (curr_time.elapsed_time().count() < time_entered*convert_micro) {   // timer is in operation
            if (count_down) {                   // counting down
                if (key == 'A') {               // time is running     
                    running_func_down();
                } else if (key == 'B') {        // time is paused
                    paused_func();
                }
            } else {                            // counting up
                if (key == 'A') {                       
                    running_func_up();
                } else if (key == 'B') {    
                    paused_func();
                }
            }
        }
        else {                                                                      // time ran out
            finished_func();
        }    
    }
    return 0;
}

void setting_func() {
    timer_screen.clear();                   
    timer_screen.setCursor(0, 0);           
    timer_screen.print(set_digit_p);        
    curr_time.reset();                      // reset the timer
    if (digit != -1) {                      // runs if user has entered a digit
        char aChar;                         // stores integer to character conversion
        if (digits_entered == 0) {          // different functionality based on digits place in timer 
            time_entered = digit;           // 1s place
            digits_entered++;               // increment the digits entered
            aChar = '0' + digit;            // convert digit (int) to char
            set_digit_p[15] = aChar;        // set the second 's' to the digit
            set_digit_p[14] = 's';
            set_digit_p[12] = 'm';
            digit = -1;                     // reset the digit
            timer_screen.setCursor(0, 0);
            timer_screen.print(set_digit_p);
        } else if (digits_entered == 1) {
            if (digit > 5) {                // makes sure user can't input the an invalid 10s digit 
                digit = -1;
                timer_screen.setCursor(0, 0);
                timer_screen.print(char_limit_p);
                ThisThread::sleep_for(1s);  // sleeps the thread to display prompt briefly
                return;
            }
            time_entered += (digit * 10);   // 10s place
            digits_entered++;
            aChar = '0' + digit;               
            set_digit_p[14] = aChar;        // set the first 's' to the digit
            digit = -1;
            timer_screen.setCursor(0, 0);
            timer_screen.print(set_digit_p);
        } else if (digits_entered == 2) { 
            time_entered += (digit * 60);   // mins place
            digits_entered++;      
            aChar = '0' + digit;
            set_digit_p[12] = aChar;        // set the 'm' to the digit
            digit = -1;
            timer_screen.setCursor(0, 0);
            timer_screen.print(set_digit_p);
        }
    }
    ThisThread::sleep_for(15ms); 
    // if 3 digits have been entered this should do nothing and just wait for the user to start
}

// the state of the LCD when the timer is paused
void paused_func() {
    digits_entered = 0;
    curr_time.stop();                       // stops the timer until started
    timer_screen.setCursor(0, 0);
    if (count_down) { timer_screen.print(down_time_paused_p); }
    else { timer_screen.print(up_time_paused_p); }
    ThisThread::sleep_for(15ms);
}

// the state of the LCD when the timer is counting down
void running_func_down() { 
    char update_time[] = "m:ss";
    digits_entered = 0;
    curr_time.start();
    timer_screen.clear();
    timer_screen.setCursor(0, 0);           
    timer_screen.print(time_remaining_p);
    int time_diff = time_entered - curr_time.elapsed_time().count()/1000000; // time difference between user inputted time and the timer
    char aChar = '0' + floor(time_diff/60);     // use floor to only display int
    update_time[0] = aChar;
    time_diff -= floor(time_diff/60)*60;
    aChar = '0' + floor(time_diff/10);
    update_time[2] = aChar;                     // change 10s slot
    time_diff -= floor(time_diff/10)*10;
    aChar = '0' + floor(time_diff);
    update_time[3] = aChar;                     // change 1s slot
    timer_screen.setCursor(0, 1);               // print the time on row 1
    timer_screen.print(update_time);            // change min slot
    ThisThread::sleep_for(15ms);
}

// expansion functionality, when time is counting up
void running_func_up(){
    digits_entered = 0;
    curr_time.start();
    int s_time_entered = time_entered;          // save the time entered        
    int time_passed = curr_time.elapsed_time().count()/1000000; // time difference between user inputted time and the timer
    char aChar = '0' + floor(time_passed/60);   // use floor to only display int
    current_time_p[11] = aChar;                 // change min slot
    aChar = '0' + floor(s_time_entered/60);
    time_counting_to_p[11] = aChar;
    time_passed -= floor(time_passed/60)*60;
    s_time_entered -= floor(s_time_entered/60)*60;
    aChar = '0' + floor(time_passed/10);
    current_time_p[13] = aChar;                 // change 10s slot
    aChar = '0' + floor(s_time_entered/10);
    time_counting_to_p[13] = aChar;
    time_passed -= floor(time_passed/10)*10;
    s_time_entered -= floor(s_time_entered/10)*10;
    aChar = '0' + floor(time_passed);
    current_time_p[14] = aChar;                 // change 1s slot
    aChar = '0' + floor(s_time_entered);
    time_counting_to_p[14] = aChar;
    timer_screen.clear();
    timer_screen.setCursor(0, 0);               // display the current time on the top of the LCD          
    timer_screen.print(current_time_p);
    timer_screen.setCursor(0, 1);               // display the time counting to on the bottom of the LCD
    timer_screen.print(time_counting_to_p);
    ThisThread::sleep_for(15ms);
}

// the state of the LCD when time has ran out
void finished_func() {
    GPIOA->ODR |= 0xc0;                     // turn on the green LEDs
    digits_entered = 0;
    curr_time.stop();
    timer_screen.clear();
    timer_screen.setCursor(0, 0);
    if (count_down) { timer_screen.print(times_up_p); } // if counting down print times up
    else { timer_screen.print(time_reached_p); }        // if counting up print times reached
    ThisThread::sleep_for(15ms);
    GPIOA->ODR &= ~(0xc0);                  // turn off the green LEDs, only really turns off when 'D' is pressed again 
}

// poll to see which key, if any, has been pressed
void keypad_poll() {
    row = 1;                        // row 1 is being set High
    GPIOC->ODR |= 0x100;            // set row 1 High
    GPIOC->ODR &= ~(0xe00);         // set other rows low
    row = 2;
    GPIOC->ODR |= 0x200;        
    GPIOC->ODR &= ~(0xd00);    
    row = 3;
    GPIOC->ODR |= 0x400;        
    GPIOC->ODR &= ~(0xb00);   
    row = 4;
    GPIOC->ODR |= 0x800;      
    GPIOC->ODR &= ~(0x700);    
}

// col1 
void col1_key(){
    col1.disable_irq();             // disable this interrupt to negate bounce
    GPIOA->ODR |= 0x20;             // turn on the red LED
    if (row == 1) {                 // figure out which row the key corresponds too
        key = '1';                  // set the key entered
        digit = 1;                  // set the digit entered
    } else if (row == 2) {
        key = '4';
        digit = 4;
    } else if (row == 3) {
        key = '7';
        digit = 7;
    } else {
        key = '*';
    }
    wait_us(400000);                // wait for bounce to settle down
    GPIOA->ODR &= ~(0x20);          // turn off the red LED
    col1.enable_irq();              // re-enable the interrupt
}

void col2_key(){
    col2.disable_irq();
    GPIOA->ODR |= 0x20;        
    if (row == 1) {
        key = '2';
        digit = 2;
    } else if (row == 2) {
        key = '5';
        digit = 5;
    } else if (row == 3) {
        key = '8';
        digit = 8;
    } else {
        key = '0';
        digit = 0;
    }
    wait_us(400000);
    GPIOA->ODR &= ~(0x20);
    col2.enable_irq();
}

void col3_key(){
    col3.disable_irq();
    GPIOA->ODR |= 0x20;
    if (row == 1) {
        key = '3';
        digit = 3;
    } else if (row == 2) {
        key = '6';
        digit = 6;
    } else if (row == 3) {
        key = '9';
        digit = 9;
    } else {
        key = '#';
    }
    wait_us(400000);
    GPIOA->ODR &= ~(0x20);
    col3.enable_irq();
}

void col4_key(){
    col4.disable_irq();
    GPIOA->ODR |= 0x20;
    if (row == 1) {
        key = 'A';
    } else if (row == 2) {
        key = 'B';
    } else if (row == 3) {
        switch_direction = true;   // flip the counting direction
        key = 'C';
    } else {
        key = 'D';
    }
    wait_us(400000);
    GPIOA->ODR &= ~(0x20);
    col4.enable_irq();
}
