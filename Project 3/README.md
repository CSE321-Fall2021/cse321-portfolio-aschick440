-------------------
About
-------------------
Project Description:  
_This program will function as an embedded pictionary game withair drawing functionality. The users can make guesses and tallypoints by using the keypad. The LED Matrix will always display the drawers picture by monitoring arm motions with IRS._  
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

* If all the lights on the dot matrix light up when the program starts running, unplug the usb and plug it back in
* When setting up, it is recommended to refer to the system schematic in the project document for a visual representation
* Depending on which direction the dot matrix is facing, the IRs will move the cursor in different directions, so it doesn't matter which Douts connect to which pins

---

--------------------
p1_code_provided.cpp:
--------------------
**THESE ARE JUST INITIAL THOUGHTS ON LAYOUT**
The program will begin in an initialization phase where it waits for the user’s location data. Then, the next initialization will be on relative DTH11 sensor mapping, the program needs these two user inputs to begin. Once these have been given, the final initialization will take place which computes how the dot matrix will display based on positioning of each DTH11 sensor. Every ~30 minutes there will be a ticker attached to an ISR to grab the current weather data. Outside of that, one thread will be constantly updating the dot map while the other thread computes the safe humidity levels and relative humidity levels of each room. If the humidity levels become too unsafe there will be an interrupt triggered that sets off the Buzzer and gives the user an LCD prompt to turn on the humidifier. Until a safe level is restored the buzzer will not turn off. 

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
