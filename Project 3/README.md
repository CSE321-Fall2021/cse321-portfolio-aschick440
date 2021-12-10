-------------------
About
-------------------
Project Description:  
_This program will function as an embedded pictionary game with air drawing functionality. The users can make guesses and tally points by using the keypad. The LED Matrix will always display the drawers picture by monitoring arm motions with IRs._  
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
 
---
**NOTE**

* When setting up, it is recommended to refer to the system schematic in the project document for a visual representation
* Depending on which direction the dot matrix is facing, the IRs will move the cursor in different directions, so it doesn't matter which IR Douts connect to which pins
* If all the lights on the dot matrix light up when the program starts running, unplug the usb and plug it back in

---

--------------------
CSE321_project3_aschick2_main.cpp:
--------------------
The program is an RTOS system that incorporates threading, synchronization, error correction, interrupts, and physical components that are used as digital inputs and outputs. At any one time there can be up to 4 threads running in parallel. Individual threads handle keypad polling, IR polling, Matrix updates, and the pictionary game state. The area of each threads activity is very clearly commented within the code. The top of the file includes the header, included libraries, all the Global state, and initializations on component Objects.

----------
Things Declared
----------
**N/A**

----------
API and Built In Elements Used
----------
* Open Weather Map API
* Open Source C++ JSON library
* Threading and synchronization

----------
Custom Functions
----------
**N/A**
