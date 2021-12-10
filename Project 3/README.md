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


// LED Matrix state
const uint8_t get_hex_blink[8] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80}; // grab col
const int max_row = 7;                                                  // row size of the matrix
const int max_col = 7;                                                  // col size of the matrix 
uint8_t matrix_state[8] = {0,0,0,0,0,0,0,0};                            // state of LED Matrix in : Idx = row, data stored 1's are cols lit
uint8_t current_digit = Max7219::MAX7219_DIGIT_0;                       // 7219 row definition object
bool pen_down = false;                                                  // state of the pen
bool eraser_down = false;                                               // state of the eraser
int cursor_col = 0;                                                     // cursors col postion
int cursor_row = 0;                                                     // cursors row position

// Game state
const int guess_time = 25;                                              // time during round
const int prep_time = 5;                                                // time between rounds   
const int us_to_s = 1000000;                                            // microseconds to seconds
const int total_rounds = 3;                                             // defines # of rounds in each game
const uint32_t TIMEOUT_MS = 30000;                                      // watchdog timeout, watchdog is only ran within the game state
bool game_t_start = true;                                               // set to false when game starts
int current_round = 1;                                                  // current round within the game
int player_guessing = -1;                                               // player with a guess in                     
int players_score[3] = {0,0,0};                                         // the current score of each player
Timer stage_time;                                                       // timer used for guessing and drawing stages

// LCD state
const char four_to_start[17] = "press 4 to start";                      // start game prompt
const char game_over[11] =     "Game Over!";                            // game over prompt
char current_time[3] =         "ss";                                    // current time left
char scores[17] =              "  P1=0,P2=0,P3=0";                      // players scores
char round_starts_in[13] =     "R1 starts in";                          // start game prompt

// Keypad state
char key = 'X';                                                         // key pressed, X represents a don't care
int key_pad_row;                                                        // keypad row that is currently low

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


// Update cursor position : ran on main thread
void decipher_max_digit();                                              
void cursor_up();                                                       
void cursor_left();
void cursor_down();
void cursor_right();

// controls individual LED blinking : ran on blink_cursor_t thread
void cursor_blink();                                                    
void turn_on_LED();
void turn_off_LED();

// monitors keypad inputs : ran on keypad_poll_t thread
void keypad_poll();                                                     

// controls the game functionality : ran on game_t thread
void decipher_key();                                                  
void between_rounds();                                                 
void during_rounds();                                                
bool check_player_guess();                                          

// ISR functions called on key presses
void col1_key();                        
void col2_key();                        
void col3_key();                        
void col4_key();
