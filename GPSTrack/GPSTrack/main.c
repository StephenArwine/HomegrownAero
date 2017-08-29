/*
 * GPSTrack.c
 *
 * Created: 8/27/2017 4:54:45 PM
 * Author : ancar
 */ 


#include "sam.h"
#include <util.h>
#include <port.h>

const static Pin LedPin = {.group = 0, .pin = 27 };


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	GclkInit();
	delayInit();
	
	pinOut(LedPin);
	pinHigh(LedPin);

    /* Replace with your application code */
    while (1) 
    {
		pinToggle(LedPin);
		delay_ms(1000);
    }
}
