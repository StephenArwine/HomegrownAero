/*
* GPSTrack.c
*
* Created: 8/27/2017 4:54:45 PM
* Author : ancar
*/


#include "sam.h"
#include <util.h>
#include <services.h>
#include <drivers.h>



const static Pin LedPin = {.group = 0, .pin = 27 };


int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	GclkInit();
	delayInit();
	sercomUSARTInit(SERCOM5,9600);
	
	GPS myMTK3329;

	
	pinOut(LedPin);
	pinHigh(LedPin);

	uint8_t message[255];

	/* Replace with your application code */
	while (1)
	{
		pinToggle(LedPin);
		
	//	delay_ms(1000);
		uint8_t bytesRead = usartDataRead(SERCOM5,message,255);
		
		//MTK3329ParseMessage(&myMTK3329,&message[0]);
	}
}
