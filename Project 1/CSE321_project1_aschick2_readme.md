About
-------------------
Project Description:
This program will prodcue a dual functional alarm that can 
either countdown or count up. The alarm will be displayed on an 
LCD where users can input the time and control the timers' 
behavior assuming the peripherals are properly hooked up.
--------------------
Contributor List
--------------------
Andrew Schick
--------------------
Features
--------------------
•	Times can be set from 0:00 to 9:59 in terms of m: ss at any time, but cannot exceed 9:59
•	Once a time is set, the user can use that saved time repeatedly until they change it. This even works when changing counting direction
•	Dual counting functionality: count up or count down
•	When switching between counting directions, a brief prompt will notify the user that the counting direction has been changed
•	Pausing at any time while the clock is running will prompt the user of a pause and stop the clock
•	LCD will continuously reflect the state of the clock
•	Whilst counting up the LCD will display the time passed and the target time
•	Whilst counting down the LCD will display the time remaining
•	When the target time is reached in either direction their will be a notification prompt and two green LEDs will light up and stay on until a new timer is started
•	Whenever a button is pressed a red LED will briefly blink
•	For the 10s seconds place, the user will be blocked from entering a number greater than five and notified of the error
--------------------
Required Materials
--------------------
Nucleo L4R5ZI
USB cable
Jumper cables
Resistors
LCD
LEDs
Keypad
Breadboard
--------------------
Resources and References
--------------------
UB CSE 321 Course Lecture Slides
	https://drive.google.com/drive/folders/1HSv1SSnKlJqvCXl3hbqz8I6bmRXM0QZy
Nucleo L4R5ZI pin layout diagram
        https://www.st.com/resource/en/user_manual/dm00368330-stm32-nucleo-144-boards-mb1312-stmicroelectronics.pdf
MBED OS baremetal reference sheet
        https://os.mbed.com/docs/mbed-os/v6.15/apis/index.html 
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
--------------------
Getting Started 
--------------------
Use the pin layout diagram given in the references to find where the specified pins are on the Nucleo.
Set up the LEDs by placing each one in series with an individual resister on the breadboard while placing the LEDs cathode (short side) on the blue rail. 
Ground the blue rail by connecting a jumper between the blue rail and a ground pin on the Nucleo. Now use a jumper to connect PA_5 to the resistor in series 
with the red LED. Then connect PA_6 and PA_7 to the resistors in series with the green LEDs. The LEDs are setup. To setup the LCD use jumpers to, connect 
Vcc to 3.3V, gnd to a gnd pin, SDA to the SDA pin, and SCL to the SCL pin. The LCD is setup. Finally, to set up the keypad look up the pinout for the 
specified 4x4 keypad. With the pinout resource and jumpes, connect Col1 to PD_7, Col2 to PD_6, Col3 to PD_5, and Col4 to PD_4. Then connect Row1 to PC_8,
Row2 to PC_9, Row3 to PC_10, and Row4 to PC_11. Now the alarm clock should be fully functional. Connect the Nucleo to a computer using the USB cable. Run. 
--------------------
CSE321_project2_aschick2_main.cpp:
--------------------
When run, the porgram will prompt the user to enter a time through the keypad. The time will be entered starting from 1's seconds to 10's seconds to minutes.
Once the user has their intended time entered, they can press A to start counting down. At anytime the timer can be paused by pressing B and unpaused by 
pressing A. In the user would like to reset the timer they can press D, then A, this will use the saved timer time. If the user would like a new time they
can press D, followed by new digits. Lastly, the user can switch between counting down and counting up by pressing the C key, which they can click whenever.
Each time the user enters a key the red LED will quickly blink. If the target time is reached the two green LEDs will stay light until a new timer is started. 
----------
Declarations
----------
Functions:
	void setting_func();
	void paused_func();
	void running_func_down();
	void running_func_up();
	void finished_func();
	void keypad_poll();
ISR Functions:
	void col1_key();            
	void col2_key();            
	void col3_key();            
	void col4_key();            	
Global Values:
	const char times_up_p[] = "times up!";                      
	const char down_time_paused_p[] = "paused at:     ";     
	const char up_time_paused_p[] = "paused at: ";             
	const char char_limit_p[] = "number exceeds 5";         
	const char time_reached_p[] = "time reached!";            
	const char counting_up_p[] = "counting up";               
	const char counting_down_p[] = "counting down";
	const int convert_micro = 1000000;
	CSE321_LCD timer_screen( 1, 16,  LCD_5x10DOTS, D14, D15);        
Global Variables:
	char set_digit_p[] = "enter time: m:ss";                  
	char time_remaining_p[] = "time remaining:";             
	char current_time_p[] = "curr time: m:ss";                 
	char time_counting_to_p[] = "counts to: m:ss";
	Timer curr_time;         
	int time_entered;         
	int row;              
	char key = 'D';             
	int digits_entered = 0;  
	int digit = -1;
	bool switch_direction = false;  
	bool count_down = true;           
APIs/Built In Elements:
	InterruptIn col1(PD_7, PullDown);
	InterruptIn col2(PD_6, PullDown);
	InterruptIn col3(PD_5, PullDown);
	InterruptIn col4(PD_4, PullDown);
	CSE321_LCD timer_screen( 1, 16,  LCD_5x10DOTS, D14, D15);
	Timer curr_time;
	ThisThread::sleep_for(1s);
	wait_us(400000);
Libraries:
	#include "mbed.h"
	#include "1802.h"
	#include <cstdio>
	#include <ctime>
----------
API and Built In Elements Used
----------
	InterruptIn (rise(), irq_enable, irq_disable)
	LCD display (clear(), print(), setCursor(a,b))
       	ThisThread (sleep_for())
	Timer (begin(), start(), reset(), stop())
	wait_us
----------
Custom Functions
Note - Global references only references global variables
void setting_func();
	Functionality:
     	Sets the timer to the digits being set
	Inputs:
		None
	Global References:
		curr_time
		digit
		digits_entered
		set_digit_p

void paused_func();
	Functionality:
     	Pauses the timer and displays the timer paused at
	Inputs:
		None
	Global References:
		digits_entered
		curr_time
		count_down

void running_func_down();
	Functionality:
     	Updates the current time on the LCD when counting down
	Inputs:
		None
	Global References:
		digits_entered
		curr_time
		time_entered

void running_func_up();
	Functionality:
     	Updates the current time of the LCD when counting up
	Inputs:
		None
	Global References:
		digits_entered
		curr_time
		time_entered
		current_time_p
		time_counting_to_p

void finished_func();
	Functionality:
     	Displays times up or time reached when the target time is reached
	Inputs:
		None
	Global References:
		digits_entered
		curr_time
		count_down

void keypad_poll();
	Functionality:
     	Constantly polls to see if the keypad had new inputs
	Inputs:
		None
	Global References:
		row

void col1_key(); 
	Functionality:
     	ISR called when col1 key is pressed to determine key
	Inputs:
		None
	Global References:
		row
		digit
		key		

void col2_key();
	Functionality:
     	ISR called when col2 key is pressed to determine key
	Inputs:
		None
	Global References:
		row
		digit
		key
           
void col3_key();
	Functionality:
     	ISR called when col3 key is pressed to determine key
	Inputs:
		None
	Global References:
		row
		digit
		key
           
void col4_key();
	Functionality:
     	ISR called when col4 key is pressed to determine key
	Inputs:
		None
	Global References:
		row
		key
		switch_direction
