# Disability Friendly Pictionary
-------------------
About
-------------------
Project Description:  
_This program will function as an embedded pictionary game with air drawing functionality. The users can make guesses and tally points by using the keypad. The LED Matrix will always display the drawers picture by monitoring arm motions with IRs. This game can accommodate from 2 to 4 players._  
Contribitor List:  
_Andrew Schick_

--------------------
Features
--------------------
* Keypad for to change game state and pen functionality
* Continuously updating LED Dot Matrix to display the drawers picture
* Blinking cursor LED
* Eraser and pen up functionality
* LCD to display current score, round, and time left
* 4 IR sensors used to draw on the Dot Matrix
* Ability to replay indefinitely

--------------------
Required Materials
--------------------
* Nucleo L4R5ZI
* Female to female jumper wires
* Male to male jumper wires
* 4 IR sensors
* 1802 LCD
* Breadboard
* Keypad
* 7219 LED Dot Matrix
* Micro USB cable

--------------------
Resources and References
--------------------
* Nucleo User manual
* Nucleo Pin Layout
* 1802 LCD Library
* 7219 Dot Matrix Library
* Standard C++ Library
* MBED RTOS Documentation
* CSE321 Lecture Slide Deck

---
**NOTE**

All reference links are present in the main.cpp file

---

--------------------
Getting Started
--------------------
* Connect the Nucleo to your computer using the USB.
* Connect 3.3V or 5V to the + rail on the breadboard
* Connect gnd to the - rail on the breadboard
* Feel free to connect all component Vcc pins to the + rail and gnd pins to the - rail
* Keypad : col4 -> PE_2, col3 -> PE_4, col2 -> PE_5, col1 -> PE_6, row2 -> PF_8, row1 -> PF_7
* LCD : SDA -> SDA, SCL -> SCL
* IR sensors : PC_0, PC1, PC_3, PC_4
* Dot Matrix : DIN -> PA_7, CS -> PD_14, CLK -> PA_5
* Clean build and run
* Keypad inputs
  * Player X wants to guess : '1', '2', '3'
  * Start game : '4'
  * Incorrect : '5'
  * Correct : '6'
  * Invert pen : 'A'
  * Invert eraser : 'B'
* Follow directions on the LCD to play a game
 
---
**NOTE**

* When setting up, it is recommended to refer to the system schematic in the project document for a visual representation
* Depending on which direction the dot matrix is facing, the IRs will move the cursor in different directions, so it doesn't matter which IR Douts connect to which pins
* If all the lights on the dot matrix light up when the program starts running, unplug the usb and plug it back in

---

--------------------
CSE321_project3_aschick2_main.cpp:
--------------------
The program is an RTOS system that incorporates threading, synchronization, timing, error correction, interrupts, and physical components that are used as digital inputs and outputs. At any one time there can be up to 4 threads running in parallel. Individual threads handle keypad polling, IR polling, Matrix updates, and the pictionary game state. The area of each threads activity is very clearly commented within the code. The top of the file includes the header, included libraries, all the Global state, and initializations on component Objects.

----------
Things Declared
----------
### Global Constants  
const uint32_t TIMEOUT_MS = 30000;  
const uint8_t get_hex_blink[8] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};  
const char four_to_start[17] = "press 4 to start";                
const char game_over[11] =     "Game Over!";  
const int max_row = 7;                                                 
const int max_col = 7;  
const int guess_time = 25;                                              
const int prep_time = 5;                                                
const int us_to_s = 1000000;                                           
const int total_rounds = 3;                                           
  
### Global Variables  
uint8_t matrix_state[8] = {0,0,0,0,0,0,0,0};                              
uint8_t current_digit = Max7219::MAX7219_DIGIT_0;                         
bool pen_down = false;                                                    
bool eraser_down = false;                                                                                                           
bool game_t_start = true;                                                                                   
char current_time[3] =         "ss";                               
char scores[17] =              "  P1=0,P2=0,P3=0";                  
char round_starts_in[13] =     "R1 starts in";                          
char key = 'X';                                              
int key_pad_row;
int current_round = 1;                                                  
int player_guessing = -1;                                                               
int players_score[3] = {0,0,0};  
int cursor_col = 0;                                                       
int cursor_row = 0;  
  
### Threading and Synchronization  
Mutex protect_matrix_state;                                        
Thread blink_cursor_t;                                            
Thread keypad_poll_t;                                                  
Thread game_t;                                                                                       
  
### Objects   
Max7219 LED(PA_7,PA_6,PA_5,PD_14);                                      
CSE321_LCD LCD( 1, 16,  LCD_5x10DOTS, D14, D15);  
Timer stage_time;                                                       
    
----------
API and Built In Elements Used
----------
* 7219 Matrix Library
* 1802 LCD library
* DigitalIn
* Thread
* Mutex
* Watchdog
* InterruptIn
* Timer

----------
Custom Functions
----------
### main thread runs on  
void decipher_max_digit();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Finds the row object for the cursors current row  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Updates : current_digit  
void cursor_up();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Moves the cursor up
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;updates current_row  
void cursor_left();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Moves the cursor left  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;updates current_col  
void cursor_down();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Moves the cursor down
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;updates current_row  
void cursor_right();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Moves cursor right 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;updates cursor_col  
  
### blink_cursor_t thread runs on    
void cursor_blink();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Makes the cursor blink and updates LEDs  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void turn_on_LED();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void turn_off_LED();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
  
### keypad_poll_t thread runs on  
void keypad_poll();                                                       
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
  
### game_t thread runs on  
void decipher_key();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void between_rounds();          
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void during_rounds();           
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
bool check_player_guess();      
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
  
### ISRs  
void col1_key();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void col2_key();                
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void col3_key();                
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
void col4_key();  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  
