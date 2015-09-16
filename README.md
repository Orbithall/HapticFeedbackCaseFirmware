## Haptic Feedback Case: The Firmware
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
- [x] add rx to change between 1 sensor and all sensors on tx, & to turn all sensors off
- [x] related to above point: have a boolean for each sensor, initialises to 0. host sends value that determines (bits of a  byte) what sensors to activate. main.c cycles this byte - you can determine what sensors you're reading since you know the initial value, and from there, the host takes control of the byte
- [ ] related to above point: echo back a sensor state change on the rx and assert it's the same as the hosts Tx, else host receives an illegal number to signify a false reading, return error message and try again 
- [ ] test rx with computer terminal 
- [ ] Add sleep states when no sensors are being Tx'd:
> When all sensors are off then turn chip select etc on mux off/ADEN etc functions of uC off except uart.  
> - manually 0 V all ouptus to uC and float all inputs (except TX RX) 
> - decrease the clock (don't brick your hardware ben)

## License 

This code is available under the GNU V3 license. 