-------------------
About
-------------------
Project Description:
This program sets an LED to blink blue until the user presses and releases a button in which case the LED will turn off and stop blinking.  

Contributor List:
Andrew Schick
--------------------
Features
--------------------
Blinking LED
Button
--------------------
Required Materials
--------------------
Nucleo L4R5ZI
USB cable
--------------------
Resources and References
--------------------
CSE 321 Project 1 broken code
--------------------
Getting Started
--------------------
Use the USB cable to connect the PC with the program file on it to the microcontroller. Run the Program. 
--------------------
CSE321_project1_aschick2_corrected_code.cpp:
--------------------
When run, the program will call blinking_LED which produce a blinking blue LED that will remain on for 2 seconds and off for .5 seconds and will continue indefinitely. When the user clicks BUTTON1 on the microcontroller the program will prepare to end within the set_termination module. Once the button is released the exit_blink module will be called as which time the program will end and the LED will turn off.
----------
Declarations
----------
Functions:
	Void blinking_LED()
	Void set_termination()
	Void exit_blink()
Variables:
	Int blink_cond
	Int terminate_thread
APIs/Built In Elements:
	Thread controller
	InterruptIn button_pressed(BUTTON1)
	Digital blue_LED(LED2)
Libraries:
	“Mbed.h”
	<cstdlib>
----------
API and Built In Elements Used
----------
	Thread
	DigitalOut
	InterruptIn
	LED2
	BUTTON1
----------
Custom Functions
----------
blinking_LED():
	Functionality:
       Indefinitely loops to create a blinking LED 
	Inputs:
		None
	Global References:
		blink_cond
		blue_LED
		printf

set_termination():
	Functionality:
		Sets flag to allow blinking to terminate when button pressed
	Inputs:
		None
	Global References:
		terminate_thread

exit_blink():
	Functionality:
		Sets flag to terminate blinking after button is released
	Inputs:
		None
	Global References:
		terminate_thread
		blink_cond
