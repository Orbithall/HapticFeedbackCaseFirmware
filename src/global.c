
#include "global.h"

FILE uart_str = FDEV_SETUP_STREAM(uart0_fputchar_int, NULL, _FDEV_SETUP_RW);