** UNDER PROGRESS, MANY SPECIFICS ARE MISSING**
** specifically pin/library usage and breadboard setup **
-------------------
About
-------------------
Project Description:
Countdown alarm displayed on an LCD where users can input the time and control the timers’ behavior.
--------------------
Contributor List
--------------------
Andrew Schick
--------------------
Features
--------------------
Static LEDs
LCD timer display
Keypad
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
RM0432 Reference manual (Nucleo L4R5ZI reference)
JDH_1804_Datasheet.pdf (LCD operational manual) 
1802.cpp (given LCD code)
1802.h (given LCD library)
LCD Library/API instructions
--------------------
Getting Started 
--------------------
Use the USB cable to connect the PC with the program file on it to the microcontroller. Connect 1 red LED in series with a resistor and ground the connection (this will be used to signal a digit being entered, the port/pin is not defined yet). Connect 1 blue and 1 green LED in series with a resistor and ground the connection (this will be used to signal the time being up, the port/pin is not defined yet). Use the bus supplied with the LCD to connect it to the Nucleo. Connect the keypad to 8 jumper cables and connect all the jumper cables to the same port at different pins. 
--------------------
CSE321_project2_aschick2_stage2_code.cpp:
--------------------
When run the program will instantly jump to the setting_timer ISR via the D interrupt. From there the user will be prompted to enter a time limit on the LCD which is initialized to 0:00 (minutes: seconds seconds), whenever a digit is entered the red LED will light up. From the setting_timer ISR only one interrupt can be set off, A (count_down). Now the LCD displays ‘time remaining x:xx’ (with the x’s being the digits set) and every second this will be decremented by 1 second. From this state three separate interrupts can be triggered: B (timer_paused), 0 (timer_finished) and D (setting_timer). The 0 interrupt will be triggered when the timer on the LCD reaches 0:00, the B interrupt will be triggered when B is pressed on the keypad, and the D interrupt will be triggered when D is pressed on the keypad. Timer_paused will freeze the countdown on the LCD and can only be exited by the A interrupt. Timer_finished will be triggered when the time remaining is 0:00 and display ‘times up’ on the LCD while the blue and green LEDs will light up. This state can only be exited with the D interrupt.      
----------
Declarations
----------
Functions:
	Void main()
	Void setting_timer()
	Void counting_down()
	Void timer_paused()
	Void timer_finished()
Values:
	Char* set_digit_p = “enter prompt”
	Char* time_remaining_p = “time remaining”
	Char* times_up_p = “times up”
Variables:
	Int curr_time
APIs/Built In Elements:
	InterruptIn D_inter(some port/pin)
	InterruptIn A_inter(some port/pin)
	InterruptIn B_inter(some port/pin)
	InterruptIn Zero_inter(some port/pin)
Libraries:
	“Mbed.h”
	 1802.h
----------
API and Built In Elements Used
----------
	InterruptIn
	LCD display (clear(), print(), setCursor(a,b))
       ThisThread (sleep_for())
----------
Custom Functions
InterruptIn D_inter(some port/pin)
       Functionality:
     	User can input a time for the timer to countdown from.
	Inputs:
		None
	Global References:
		Set_digit_p
		InterruptIn A_inter
		Curr_time

InterruptIn A_inter(some port/pin)
       Functionality:
     	The timer will begin counting down by 1 second per second.
	Inputs:
		None
	Global References:
		Time_remaining_p
		Curr_time
		B_inter
		D_inter
		Zero_inter
InterruptIn B_inter(some port/pin)
       Functionality:
     	The timer will be paused.
	Inputs:
		None
	Global References:
		A_inter
InterruptIn Zero_inter(some port/pin)
       Functionality:
     	The time has reached 0.
	Inputs:
		None
	Global References:
		D_inter
		Times_up_p


