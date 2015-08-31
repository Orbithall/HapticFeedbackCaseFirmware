# TO-DO:
--> fix headers and includes to make new makefile compile the code

--> put output data into bin, source data into src

--> add rx to change between 1 sensor and all sensors on tx, & to turn all sensors off (new state in FSM + boolean for each sensor, if true for sensor n, then tx)

--> test rx with computer terminal 

--> Add varying sleep states i.e. when all sensors are off sleep

--> adapt app 

--> refactor app 

--> look into making a service

# HapticFeedbackCaseFirmware
Firmware for the haptic feedback case to be programmed to board rev.10

Use the makefile to clean/build/program the code to the microcontroller. 

The makefile is configurated to use an arduino as the ISP.
