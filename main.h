#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include "serial.h"
#include "init.h"
#include "control.h"

#define MAIN_PACKET_END_BYTE           0xFF

uint8_t sensor; 

int main(void);

#endif