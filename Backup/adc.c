#include "adc.h"

uint8_t ADCread(void)
{ 
	ADCSRA |= 1<<ADEN;		//turn on ADC feature
	ADCSRA |= 1<<ADSC;	//start first conversion
	while(ADCSRA&0x40); //start and wait until conversion complete  
	return ADCH;
} 

void ADCset(uint8_t mux) { //separate function to avoid setting ADC mux to the same channel for each iter in the pad loop
	ADMUX = (ADMUX & 0b11100000) | (mux & 0b00011111);    //MUX 0 to 4
	mux >>= 2;
	ADCSRB = (ADCSRB & 0b11110111) | (mux & 0b00001000);  //MUX 5    //0b00100000	 
}