## Haptic Feedback Case: The Firmware
> The haptic feedback case is a shell that attaches to most modern android phones, it interfaces the user through positional and pressure input on the sides and rear of their device.   

> This firmware is to be programmed to the rev.10 board designed and developed by Ben Kazemi. 

## Dependencies
- WinAVR or derivative package to build .hex
- avrdude or alternative to program .hex 

## Build
Currently the makefile is defined to program the board via ArduinoISP, and must have the communication port configured appropriately. 

`make clean && make && make program`

## TODO List:
- [ ] fix headers and includes to make new makefile compile the code
- [ ] Refactor where source and output are stored
- [ ] put output data into bin, source data into src
- [ ] add rx to change between 1 sensor and all sensors on tx, & to turn all sensors off (new state in FSM + boolean for each sensor, if true for sensor n, then tx)
- [ ] test rx with computer terminal 
- [ ] Add varying sleep states i.e. when all sensors are off sleep
- [ ] Android App: adapt app 
- [ ] Android App: refactor app 
- [ ] Android App: look into making a service

## License 

This code is available under the GNU V3 license. 
