#ifndef MAIN_H
#define MAIN_H

#include "cpu.h"
#include <avr/io.h>
#include <stdio.h>
#include "serial.h"
#include "global.h"
#include "init.h"
#include "control.h"

#define MAIN_PACKET_END_BYTE           0xFF

uint8_t sensor = 0; 

int main(void);

#endif