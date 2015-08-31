#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

uint8_t ADCread(void);
void ADCset(uint8_t);

#endif