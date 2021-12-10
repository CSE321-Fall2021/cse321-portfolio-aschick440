-------------------
About
-------------------
Project Description:  
* _This program will function as an embedded pictionary game withair drawing functionality. The users can make guesses and tallypoints by using the keypad. The LED Matrix will always display the drawers picture by monitoring arm motions with IRS._
Contribitor List:  
* Andrew Schick

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
* Open Weather Map API
* C++ JSON conversion library
* Nucleo User manual
* LCD User manual
* Dot matrix array manual
* Buzzer User manual
* DTH11 user manual
* C++ library documentation
* MBED API documentation

--------------------
Getting Started
--------------------
Connect the Nucleo to your computer using the USB. Determine where the 5 humidity sensors will be placed. Place the sensors in those rooms and use jumper wires to attach them to port: **PLACEHOLDER**, pins: **PLACEHOLDER** on the microcontroller. Setup the keypad by plugging in the 4 column wires to port: **PLACEHOLDER**, pins: **PLACEHOLDER** and the 4 row wires to port: **PLACEHOLDER**, pins: **PLACEHOLDER**. Then connect the LCD to Vcc, gnd, SCL, and SCA on the microcontroller. **PLACEHOLDER FOR ATTACHING THE DOT MATRIX ARRAY AND BUZZER.** Run the program. The LCD will give a prompt to enter the locations zip-code. Then it will prompt the user to input the relative locations of each DTH11 sensor. Now the dot matrix will go through an initialization phase to compute the mapping. Followed by grabbing current weather data from the weather API. The humidity sensor should be fully operational at this time.

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
