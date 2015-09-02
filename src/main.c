#include "main.h"



int main(void) {
	initStartup();
	while (1) {	
		uart0_fputchar_int(MAIN_PACKET_END_BYTE,&uart_str);
		for (sensor = 0; sensor<4; sensor++) {  
			controlGetStrip(&sensor);
		}
	    controlPadInput();	
	}
	return 0;
}