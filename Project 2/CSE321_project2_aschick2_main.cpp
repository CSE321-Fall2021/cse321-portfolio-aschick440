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
void running_func();
void finished_func();
void keypad_poll();

// ISR functions
void col1_key();            // called when col1 key pressed
void col2_key();            // called when col2 key pressed
void col3_key();            // called when col3 key pressed
void col4_key();            // called when col4 key pressed

Timer curr_time;            // time to countdown from
int time_entered;           // the number of seconds entered by user

int row;                    // row that is currently low
char key = 'D';             // key pressed, starts out in setting time state
int digits_entered = 0;     // number of digits the user has entered thus far 
int digit = -1;             // if a digit key was pressed it will be stored here
const int convert_micro = 1000000;                      // converts second to microseconds 

// LCD prompts
char set_digit_p[] = "enter time: m:ss";                    // setting_timer
char time_remaining_p[] = "time left: m:ss";                // counting_down
const char times_up_p[] = "times up!";                      // timer_finished
const char time_paused_p[] = "paused at: ";                 // timer_paused
const char char_limit_p[] = "number exceeds 5";             // overflow error


CSE321_LCD timer_screen( 1, 16,  LCD_5x10DOTS, D14, D15);   // creates LCD object

int main (){
    begin(timer_screen);                        // used to set up LCD display
    timer_screen.setCursor(0, 1);               // set the cursor to the second row
    RCC->AHB2ENR |= 0x5;                        // enable the port A and C clock
    GPIOC->MODER |= 0x550000;                   // mask pins to output to PC_8 - PC_11
    GPIOC->MODER &= ~(0xAA0000);                // mask pins to output to PC_8 - PC_11
    GPIOA->MODER |= 0x5400;                     // mask pins to output to PA_5 (key pressed) PA_6 and PA_7 () (times up)
    GPIOA->MODER &= ~(0xab00);                  // mask pins to output to PA_5, PA_6, PA_7
    col1.rise(&col1_key);                       // All interrupts enabled on rising edge
    col2.rise(&col2_key);
    col3.rise(&col3_key);
    col4.rise(&col4_key);
    while (1) {                     // runs forever
        keypad_poll();                          
        if (key == 'D' || (key >= '0' && key <= '9')) {                             // runs when time is being set
            setting_func();
        }
        else if (curr_time.elapsed_time().count() < time_entered*convert_micro) {    // runs when the time has not finished
            if (key == 'A') {
                running_func();
            } else if (key == 'B') {
                paused_func();
            }
        }
        else {
            finished_func();
        }
    }
    return 0;
}

void setting_func() {
    timer_screen.clear();                   // reset enter prompt
    timer_screen.setCursor(0, 1);
    timer_screen.print(set_digit_p);
    curr_time.reset();                      // reset the timer
    if (digit != -1) {                      // runs if user has entered a digit
        char aChar;                         // stores integer to character conversion
        if (digits_entered == 0) {
            time_entered = digit;           // 1s place
            digits_entered++;               // increment the digits entered
            aChar = '0' + digit;
            set_digit_p[15] = aChar;        // set the second 's' to the digit
            set_digit_p[14] = 's';
            set_digit_p[12] = 'm';
            digit = -1;                     // reset the digit
            timer_screen.setCursor(0, 1);
            timer_screen.print(set_digit_p);
        } else if (digits_entered == 1) {
            if (digit > 5) {
                digit = -1;
                timer_screen.setCursor(0, 1);
                timer_screen.print(char_limit_p);
                ThisThread::sleep_for(1s);
                return;
            }
            time_entered += (digit * 10);   // 10s place
            digits_entered++;
            aChar = '0' + digit;               
            set_digit_p[14] = aChar;        // set the first 's' to the digit
            digit = -1;
            timer_screen.setCursor(0, 1);
            timer_screen.print(set_digit_p);
        } else if (digits_entered == 2) { 
            time_entered += (digit * 60);   // mins place
            digits_entered++;      
            aChar = '0' + digit;
            set_digit_p[12] = aChar;        // set the 'm' to the digit
            digit = -1;
            timer_screen.setCursor(0, 1);
            timer_screen.print(set_digit_p);
        }
    }
    ThisThread::sleep_for(15ms); 
    // if 3 digits have been entered this should do nothing and just wait for the user to start
}

void paused_func() {
    digits_entered = 0;
    curr_time.stop();
    timer_screen.setCursor(0, 1);
    timer_screen.print(time_paused_p);
    ThisThread::sleep_for(15ms);
}

void running_func() { 
    digits_entered = 0;
    curr_time.start();
    int time_diff = time_entered - curr_time.elapsed_time().count()/1000000; // time difference between user inputted and the timer
    char aChar = '0' + floor(time_diff/60);
    time_remaining_p[11] = aChar;
    time_diff -= floor(time_diff/60)*60;
    aChar = '0' + floor(time_diff/10);
    time_remaining_p[13] = aChar;
    time_diff -= floor(time_diff/10)*10;
    aChar = '0' + floor(time_diff);
    time_remaining_p[14] = aChar;
    timer_screen.clear();
    timer_screen.setCursor(0, 1);           // set the LCDs cursor
    timer_screen.print(time_remaining_p);
    ThisThread::sleep_for(15ms);
}

void finished_func() {
    GPIOA->ODR |= 0xc0; 
    digits_entered = 0;
    curr_time.stop();
    timer_screen.clear();
    timer_screen.setCursor(0, 1);
    timer_screen.print(times_up_p);
    ThisThread::sleep_for(15ms);
    GPIOA->ODR &= ~(0xc0); 
}

void keypad_poll() {
    row = 1;
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
    col1.disable_irq();
    GPIOA->ODR |= 0x20;
    if (row == 1) {
        key = '1';
        digit = 1;
    } else if (row == 2) {
        key = '4';
        digit = 4;
    } else if (row == 3) {
        key = '7';
        digit = 7;
    } else {
        key = '*';
    }
    wait_us(400000);
    GPIOA->ODR &= ~(0x20);
    col1.enable_irq();
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
        key = 'C';
    } else {
        key = 'D';
    }
    wait_us(400000);
    GPIOA->ODR &= ~(0x20);
    col4.enable_irq();
}
