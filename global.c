#include <stdio.h>
#include "global.h"
#include "serial.h"

FILE uart_str = FDEV_SETUP_STREAM(uart0_fputchar, NULL, _FDEV_SETUP_RW);