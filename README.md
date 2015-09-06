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
- [ ] add rx to change between 1 sensor and all sensors on tx, & to turn all sensors off (new state in FSM + boolean for each sensor, if true for sensor n, then tx)
- [ ] test rx with computer terminal 
- [ ] Add varying sleep states i.e. when all sensors are off sleep
> When all sensors are off, 
> - manually 0 V all ouptus to uC and float all inputs (except TX RX) 
> - decrease the clock (don't brick your hardware ben)

## License 

This code is available under the GNU V3 license. 