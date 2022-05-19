## Haptic Feedback Case: The Firmware
![alt text](https://imgur.com/NIQDIgh)

> The haptic feedback case is a shell that attaches to most modern android phones, it interfaces the user through positional and pressure input on the sides and rear of their device.   

> This firmware is to be programmed to the rev.10 board designed and developed by Ben Kazemi. 

## Dependencies
- WinAVR or derivative package to build .hex
- avrdude or alternative to program .hex 

## Build
The makefile is currently defined to program the board via ArduinoISP, and must have the communication port configured appropriately. 

```
make clean
make 
make program
```

## TODO List:
- [x] fix headers and includes to make new makefile compile the code
- [x] put source data into src folder
- [x] re-write the serial lib
- [x] interrupt driven uart
- [x] interrupt driven ADC (unnecessary due to the structure of the program, hardware setup is defined, then ADC is read with processing based on the reading done immediately after - an interrupt driven ADC doesn't fit into this structure as you cannot set the ADC earlier in the code as the hardware is constantly changing with the Multiplexer etc)
- [x] add rx to implement sensor selection on tx, & to turn all sensors off
- [x] related to above point: have a boolean for each sensor, initialises to 0. host sends value that determines (bits of a  byte) what sensors to activate. main.c cycles this byte - you can determine what sensors you're reading since you know the initial value, and from there, the host takes control of the byte
- [x] echo back a sensor state change ONLY IF IT'S CHANGED on the rx and assert it's the same as the hosts Tx, else host receives an illegal number to signify a false reading, return error message and try again 
> this is taken out since it's been deemed unnecessary considering the data will be travelling through a wired medium 
- [x] test rx with computer terminal 
- [x] Add sleep states at off state 
- [x] When all sensors are off then disable adc spi & timers 
- [x] manually ground all inputs and outputs to uC (except TX RX) to keep current draw to a minimum  
- [x] decrease the clock when sleeping (don't brick your hardware ben)
- [x] decrease the baud rate when sleeping
- [x] turn decoder for mux all low if not 0b00010000
- [x] move all sleep code to sleep.c and talk to it via sleep.h

## License 

This code is available under the GNU V3 license. 
