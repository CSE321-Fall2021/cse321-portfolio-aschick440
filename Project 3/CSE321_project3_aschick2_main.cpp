/**

    File:   CSE321_project3_aschick2_main.c

    Author1:        Andrew Schick (aschick2@buffalo.edu)
    Date:           Fall 2021
    Partner:        Solo submission
    Course:         CSE 321 - Embedded Systems
    Project:        project 3 Disability Friendly Pictionary
    Modules:         
                    main, cursor_up, cursor_down, cursor_left, cursor_right, decipher_max_digit, 
                    cursor_blink, turn_on_LED, turn_off_LED,
                    decipher_key, between_rounds, during_rounds, check_player_guess,
                    keypad_poll, col1_key, col2_key, col3_key, col4_key
                                  
    Inputs:         
                    4x4 Matrix Keypad (0-6, A, B)
                    4 IR sensors
                    
    Outputs:        
                    1802 LCD
                    7219 Dot Matrix
                    
    Constraints:   
                    runs forever 
                    thread safe 
                    support parallel processes
                    timeout and restart if thread hangs
                    continously monitor arm motion
                    LEDs must reflect picture

    File Summary:
        This program will prodcue a dual functional alarm that can 
        either countdown or count up. The alarm will be displayed on an 
        LCD where users can input the time and control the timers' 
        behavior assuming the peripherals are properly hooked up.

    References:     
        UB CSE 321 Course Lecture Slides
        https://drive.google.com/drive/folders/1HSv1SSnKlJqvCXl3hbqz8I6bmRXM0QZy
        Nucleo L4R5ZI pin layout diagram
        https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf
        MBED OS RTOS reference sheet
        https://os.mbed.com/handbook/RTOS  
        Standard C++ library
        https://en.cppreference.com/w/cpp/header
        RM0432 Reference manual (Nucleo L4R5ZI reference)
        https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
        JDH_1804_Datasheet.pdf (LCD operational manual)
        https://learn-us-east-1-prod-fleet02-xythos.content.blackboardcdn.com/5e00ea752296c/10911180?X-Blackboard-Expiration=1636124400000&X-Blackboard-Signature=WJEqhTeGYBwyjhecErnD03%2FVxH7GcX4dk9iQu0CQsDk%3D&X-Blackboard-Client-Id=100310&response-cache-control=private%2C%20max-age%3D21600&response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%27JDH_1804_Datasheet.pdf&response-content-type=application%2Fpdf&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20211105T090000Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PL5SJBSTP6%2F20211105%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=13cac5f287e56358e05aeb6c8feef0b85c9a77e5d7f802a6a48d8d3ce5e25838 
        1802.cpp (given LCD code)
        https://learn-us-east-1-prod-fleet02-xythos.content.blackboardcdn.com/5e00ea752296c/10910994?X-Blackboard-Expiration=1636124400000&X-Blackboard-Signature=pZBgiCDZlUGlLHC623Hc2hUGfpog3aCUUR%2FIvXtzAFw%3D&X-Blackboard-Client-Id=100310&response-cache-control=private%2C%20max-age%3D21600&response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%271802.cpp&response-content-type=text%2Fx-c&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20211105T090000Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PL5SJBSTP6%2F20211105%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=a217e88a642dc97899ebae343b2dc122e8d5e57fdfdd4c2d73dfc94158d10e0d
        1802.h (given LCD library)
        https://learn-us-east-1-prod-fleet02-xythos.content.blackboardcdn.com/5e00ea752296c/10910995?X-Blackboard-Expiration=1636124400000&X-Blackboard-Signature=fk8ij77VqCofwkZi3NIeQVdB5GUxMpAmBW9qQSSqo%2Bw%3D&X-Blackboard-Client-Id=100310&response-cache-control=private%2C%20max-age%3D21600&response-content-disposition=inline%3B%20filename%2A%3DUTF-8%27%271802.h&response-content-type=text%2Fx-c&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20211105T090000Z&X-Amz-SignedHeaders=host&X-Amz-Expires=21600&X-Amz-Credential=AKIAZH6WM4PL5SJBSTP6%2F20211105%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=c4a1fec36c7fbd9264cbc6f9dcfdf1a89830d02d841e02ce8626babfaa065b1d
        max7219.cpp (Matrix code)
        https://os.mbed.com/teams/Maxim-Integrated/code/MAX7219//file/b5e4379a3d90/max7219.cpp/
        max7219.h (Matrix library)
        https://os.mbed.com/teams/Maxim-Integrated/code/MAX7219//file/b5e4379a3d90/max7219.h/

**/

#include "mbed.h"
#include "max7219.h"
#include "1802.h"
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <cmath>

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

// synchronization
Mutex protect_matrix_state;                                             // protects shared state : 'key', 'cursor_col', 'cursor_row', 'current_digit', 'matrix_state'                                         

// threads
Thread blink_cursor_t;                                                  // determines individual LED states
Thread keypad_poll_t;                                                   // polls the keypad
Thread game_t;                                                          // runs the drawing game 
// Thread main_t                                                        // computes cursors position

// LED Matrix Object
Max7219 LED(PA_7,PA_6,PA_5,PD_14);                                      // PA_7 = Din, PA_5 = CLK, PD_14 = cs **NOTE** : PA_6 is assigned to miso, however it is useless

// LCD Object
CSE321_LCD LCD( 1, 16,  LCD_5x10DOTS, D14, D15);

// IR Sensor pins
DigitalIn move_right(PC_1, PullDown);
DigitalIn move_left(PC_0, PullDown);
DigitalIn move_up(PC_3, PullDown);
DigitalIn move_down(PC_4, PullDown);

// Keypad Interrupts
// input pins to Nucleo (ODR): Port = E, Pins = 2,4,5,6, Cols = c1 E6, c4 E2
InterruptIn col1(PE_6, PullDown);                                       // col1 key pressed
InterruptIn col2(PE_5, PullDown);                                       // col2 key pressed
InterruptIn col3(PE_4, PullDown);                                       // col3 key pressed
InterruptIn col4(PE_2, PullDown);                                       // col4 key pressed


/*

    __________________MAIN THREAD__________________

*/
    

int main() {

    // initialize the Dot Matrix
    max7219_configuration_t Matrix_config = 
    {
        .device_number = 1,
        .decode_mode = 0,
        .intensity = Max7219::MAX7219_INTENSITY_8,
        .scan_limit = Max7219::MAX7219_SCAN_8 
    };
    LED.init_device(Matrix_config);
    LED.enable_device(1);
    for (int cols = 1; cols < 9; cols ++) { LED.clear_digit(1, cols); }

    // initialize keypad
    // output pins from Nucleo (IDR): Port = F, Pins = 7,8, Cols = r1 F7, r2 F8  
    // **NOTE** This also initilizes port B to use with a vibrator, however that functionality has not been added
    RCC->AHB2ENR |= 0x22;                                               // enable the port B and F clock
    GPIOF->MODER |= 0x14005;                                            // mask pins to output to PF_0, PF_1, PF_7, PF_8 Used for keypad
    GPIOF->MODER &= ~(0xeb005);                                         // mask pins to output to PF_0, PF_1, PF_7, PF_8
    GPIOB->MODER |= 0x45000000;                                         // mask pins to output to PB_12, PB_13, PB_15 Used for Vibrators
    GPIOB->MODER &= ~(0xba000000);                                      // mask pins to output to PB_12, PB_13, PB_15

    // initialize interrupts on rising edge
    col1.rise(&col1_key);                       
    col2.rise(&col2_key);
    col3.rise(&col3_key);
    col4.rise(&col4_key);

    // initialize LCD
    begin(LCD);                                                         
    LCD.setCursor(0, 0);                                                
    LCD.print(four_to_start);
    
    // begin polling keypad
    keypad_poll_t.start(keypad_poll);

/*
    polls IR sensor pins for motion and game initialization
*/
    while (true) 
    {
        if (key == '4' && game_t_start)                                 // check if the game has been started
        {                   
            game_t.start(decipher_key);                                 // start game_t thread 
            game_t_start = false;                                       // game runs indefinitely
        }                   
        if (!move_up.read())                                            // IR move_up(PC_3) triggered 
        {                   
            cursor_up();                    
            ThisThread::sleep_for(200ms);                               // sleep to mitigate noise
        }                   
        if (!move_left.read())                                          // IR move_up(PC_0) triggered 
        {                   
            cursor_left();                  
            ThisThread::sleep_for(200ms);                   
        }                   
        if (!move_down.read())                                          // IR move_up(PC_4) triggered 
        {                   
            cursor_down();                  
            ThisThread::sleep_for(200ms);                   
        }                   
        if (!move_right.read())                                         // IR move_up(PC_1) triggered 
        {
            cursor_right();
            ThisThread::sleep_for(200ms);
        }
    }
    return 0;                                                           // prevents memory leakage, main_t never returns
}

/*
    updates MAX7219_DIGIT Object to associate with the cursors current_row
*/
void decipher_max_digit() 
{
    switch (cursor_row) {
        case 0:
            current_digit = Max7219::MAX7219_DIGIT_0; 
            break;
        case 1:
            current_digit = Max7219::MAX7219_DIGIT_1; 
            break;
        case 2:
            current_digit = Max7219::MAX7219_DIGIT_2; 
            break;
        case 3:
            current_digit = Max7219::MAX7219_DIGIT_3; 
            break;
        case 4:
            current_digit = Max7219::MAX7219_DIGIT_4; 
            break;
        case 5:
            current_digit = Max7219::MAX7219_DIGIT_5; 
            break;
        case 6:
            current_digit = Max7219::MAX7219_DIGIT_6; 
            break;
        case 7:
            current_digit = Max7219::MAX7219_DIGIT_7; 
            break;
    }
    return;
}

/*
    current_row decremented
*/
void cursor_up()
{
    protect_matrix_state.lock();                                        // lock shared state                                         
    if (cursor_row != 0) { cursor_row--; }                              // checks boundary
    decipher_max_digit();                                               // determines new MAX_7219_DIGIT Object
    protect_matrix_state.unlock();                                      // unlock shared state
    ThisThread::yield();                                                // send signal
    return;

}

/*
    current_col incremented
*/
void cursor_left()
{
    protect_matrix_state.lock();
    if (cursor_col != max_col) { cursor_col++; }
    protect_matrix_state.unlock();
    ThisThread::yield();
    return;
}

/*
    current_row incremented
*/
void cursor_down() 
{
    protect_matrix_state.lock();
    if (cursor_row != max_row) { cursor_row++; }
    decipher_max_digit();
    protect_matrix_state.unlock();
    ThisThread::yield();
    return;
}

/*
    current_col decremented
*/
void cursor_right()
{
    protect_matrix_state.lock();
    if (cursor_col != 0) { cursor_col--; }
    protect_matrix_state.unlock();
    ThisThread::yield();
    return;
}


/*

    __________________GAME_T THREAD__________________

*/


/*
    maintains game continuity and flow
*/
void decipher_key()
{
    // initalize watchdog
    Watchdog &watchdog = Watchdog::get_instance();                      // get watchdog instance
    watchdog.start(TIMEOUT_MS);                                         // timeouts if any game stage takes too long

    // once game starts it runs indefinitely                    
    while (true)                    
    {                   
        // wait for game start                  
        while (key != '4')                  
        {                   
            Watchdog::get_instance().kick();                            // continuously kick when the game is not in play
            ThisThread::sleep_for(1s);                  
        }                   

        // reset the game state                 
        current_round = 1;                  
        for (int player = 0; player < 3; player++)                      // reset the players scores
        {                   
            players_score[player] = 0;                                  // initialize player scores to 0
            scores[(player+1)*5] = '0';                 
        }                   
        LCD.setCursor(0, 1);                    
        LCD.print(scores);                                              // display the current scores

        // order of events per round                    
        for (int imm_round = 1; imm_round <= total_rounds; imm_round++)
        {                   
            key = 'X';                                                  // reset key
            blink_cursor_t.start(cursor_blink);                         // start blinking thread MOVE TO INSIDE THE GAME FUNCTIONALITY WHEN A NEW ROUND STARTS
            between_rounds();                                           // run the prep time before round
            Watchdog::get_instance().kick();                            // kick inbetween stages
            during_rounds();                                            // while the round is in play
            Watchdog::get_instance().kick();
            current_round++;
        }

        // game over screen
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print(game_over);
        LCD.setCursor(0, 1);
        LCD.print(scores);
        ThisThread::sleep_for(5s);
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print(four_to_start);
    }
    return;
}


/*
    prep stage in between each round
*/
void between_rounds()
{
    // set the LCD to display when round and player scores
    stage_time.start();
    LCD.setCursor(0, 0);
    char round_char = '0' + current_round;
    round_starts_in[1] = round_char;
    LCD.clear();
    LCD.print(round_starts_in);
    LCD.setCursor(0, 1);
    LCD.print(scores);
    stage_time.reset();

    // loops until prep time has been surpassed
    while (stage_time.elapsed_time().count()/us_to_s < prep_time)
    {
        int time_diff = prep_time - stage_time.elapsed_time().count()/us_to_s;
        char aChar = '0' + floor(time_diff/10);                                 
        current_time[0] = aChar;                                        // update 10s slot
        time_diff -= floor(time_diff/10)*10;
        aChar = '0' + floor(time_diff);
        current_time[1] = aChar;
        LCD.setCursor(13, 0);                                           // update 1s slot
        LCD.print(current_time);                                        // update the displayed time
    }

    // clear the Matrix state for the next round
    protect_matrix_state.lock();
    for (int cols = 1; cols < 9; cols ++)                                       
    { 
        LED.clear_digit(1, cols); 
        matrix_state[cols-1] = 0;
    }         
    protect_matrix_state.unlock();
    return;
}

/*
    During round guessing and drawing state
*/
void during_rounds()
{
    // reset LCD and timer
    key = 'X';          
    LCD.clear();
    stage_time.reset();

    // loops until the guess_time had been surpassed or until a player guesses correctly
    while (stage_time.elapsed_time().count()/us_to_s < guess_time)
    {
        LCD.setCursor(0, 1);
        LCD.print(scores); 
        int time_diff = guess_time - stage_time.elapsed_time().count()/us_to_s;
        char aChar = '0' + floor(time_diff/10);
        current_time[0] = aChar;                                        // change 10s slot
        time_diff -= floor(time_diff/10)*10;
        aChar = '0' + floor(time_diff);
        current_time[1] = aChar;                                        // change 1s slot
        LCD.setCursor(0, 0);
        LCD.print(current_time);
        if (key != 'X')                                                 // key pressed
        {
            if (check_player_guess()) { return; }                       // a player guessed correctly, end the round                          
        }
    }    
    return;
}

/*
    set the player who has guessed or confirm the correctness of their guess
    RETURNS : whether round ends
*/
bool check_player_guess()
{ 
    // reset key to avoid infinite looping
    char saved_key = key;                                        
    key = 'X';           

    // jump to key                                               
    switch (saved_key) {                                        
        case '1':                                                       // P1 guessed
            player_guessing = 1;                                        
            break;                                      
        case '2':                                                       // P2 guessed
            player_guessing = 2;                                        
            break;                                      
        case '3':                                                       // P3 guessed
            player_guessing = 3;                                        
            break;                                      
        case '5':                                                       // Guess was wrong, update scores
            if (players_score[player_guessing-1] > 0) 
            { 
                players_score[player_guessing-1]--; 
                char player_score_char = '0' + players_score[player_guessing-1];
                scores[player_guessing*5] = player_score_char;   
            }
            break;
        case '6':                                                       // Guess was correct, update scores
            players_score[player_guessing-1]++;
            char player_score_char = '0' + players_score[player_guessing-1];
            scores[player_guessing*5] = player_score_char;
            return true;                                                // end round
    }
    return false;                                                       // drawer did not confirm a correct guess
}


/*

    __________________BLINK_CURSOR_T THREAD__________________

*/


/*
    make the cursor blink
*/
void cursor_blink()
{
    while(1)
    {
        protect_matrix_state.lock();
        if (eraser_down) { turn_off_LED(); }                                                // eraser on
        else if (pen_down) { turn_on_LED(); }                                               // pen on
        else                                                                                // pen off
        {
            if (matrix_state[cursor_row] & (get_hex_blink[cursor_col])) { turn_on_LED(); }  // LED already on
            else { turn_off_LED(); }                                                        // LED already off
        }
        protect_matrix_state.unlock();
        ThisThread::yield();
        ThisThread::sleep_for(200ms);
    }
}

/*
    set an LED to blink on -> off
*/
void turn_off_LED() 
{
    matrix_state[cursor_row] |= get_hex_blink[cursor_col];              // turn LED on
    LED.write_digit(1, current_digit, matrix_state[cursor_row]);        // update Matrix
    ThisThread::sleep_for(200ms);                                       // sleep to blink
    matrix_state[cursor_row] &= ~(get_hex_blink[cursor_col]);           // turn LED off
    LED.write_digit(1, current_digit, matrix_state[cursor_row]);        
}

/*
    set and LED to blink off -> on
*/ 
void turn_on_LED()
{
    matrix_state[cursor_row] &= ~(get_hex_blink[cursor_col]);
    LED.write_digit(1, current_digit, matrix_state[cursor_row]);
    ThisThread::sleep_for(200ms);
    matrix_state[cursor_row] |= get_hex_blink[cursor_col];
    LED.write_digit(1, current_digit, matrix_state[cursor_row]);
}


/*

    __________________KEYPAD_POLL_T THREAD__________________

*/


/*
    poll keypad for inputs
*/
void keypad_poll() {
    while (true)
    {   
        key_pad_row = 1;                                                // row 1 is being set High
        GPIOF->ODR |= 0x080;                                            // set row 1 High
        GPIOF->ODR &= ~(0xf7f);                                         // set other rows low
        ThisThread::sleep_for(10ms);
        key_pad_row = 2;                    
        GPIOF->ODR |= 0x100;          
        GPIOF->ODR &= ~(0xeff); 
        ThisThread::sleep_for(10ms);
        key_pad_row = 3;                    
        GPIOF->ODR |= 0x001;          
        GPIOF->ODR &= ~(0xffe); 
        key_pad_row = 4;                    
        GPIOF->ODR |= 0x002;          
        GPIOF->ODR &= ~(0xffd);    
    } 
}

/*
    col1 key pressed
*/
void col1_key(){
    col1.disable_irq();                                                 // disable interrupt
    if (key_pad_row == 1) {                                             // determine key pressed
        key = '1';                                                      // set key pressed
    } else {                                    
        key = '4';                                  
    }                                   
    wait_us(400000);                                                    // debounce time
    col1.enable_irq();                                                  // re-enable interrupt
}

/*
    col2 key pressed
*/
void col2_key(){
    col2.disable_irq();
    if (key_pad_row == 1) {
        key = '2';
    } else {
        key = '5';
    } 
    wait_us(400000);
    col2.enable_irq();
}

/*
    col3 key pressed
*/
void col3_key(){
    col3.disable_irq();
    if (key_pad_row == 1) {
        key = '3';
    } else {
        key = '6';
    }
    wait_us(400000);
    col3.enable_irq();
}

/*
    col4 key pressed
*/
void col4_key(){
    col4.disable_irq();
    if (key_pad_row == 1) {
        key = 'A';
        pen_down = !pen_down;                                           // invert pen state
    } else {
        key = 'B';
        eraser_down = !eraser_down;                                     // invert eraser state
    }
    wait_us(400000);
    col4.enable_irq();
}