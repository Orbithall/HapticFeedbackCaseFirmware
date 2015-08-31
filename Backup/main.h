#ifndef MAIN_H
#define MAIN_H

#include "cpu.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "serial.c"
#include "global.c"
#include "helper.c"
#include "init.c"
#include "adc.c"
#include "control.c"

#define MAIN_PACKET_END_BYTE           0xFF

uint8_t sensor = 0; 

int main(void);

#endif