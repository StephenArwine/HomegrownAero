/*
* GPSTrack.c
*
* Created: 8/27/2017 4:54:45 PM
* Author : ancar
*/


#include "sam.h"
#include <util.h>
#include <services.h>

const static Pin LedPin = {.group = 0, .pin = 27 };


int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	GclkInit();
	delayInit();
	USART_init(9600);
	
	
	
	
	pinOut(LedPin);
	pinHigh(LedPin);

	uint8_t message[255];

	/* Replace with your application code */
	while (1)
	{
		pinToggle(LedPin);
		delay_ms(1000);
		USARTDataRead(USART_0,message,255);
	}
}
