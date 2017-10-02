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

uint8_t interuptData[255];
bool newMessage = false;


const static Pin LedPin = {.group = 0, .pin = 27 };

void SERCOM5_Handler(void) ;

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    GclkInit();
    delayInit();
    sercomUSARTInit(SERCOM5,9600);

    GPS aMTK3329;
    GPS *myMTK3329;

    myMTK3329 =&aMTK3329;

    pinOut(LedPin);
    pinHigh(LedPin);

    uint8_t message[255];

    /* Replace with your application code */
    while (1) {
        pinToggle(LedPin);

        //	delay_ms(1000);
        //uint8_t bytesRead = usartDataRead(SERCOM5,message,255);

        //  MTK3329ParseMessage(myMTK3329,&message[0]);
          message[1] = 0;

        if (newMessage == true) {
            MTK3329ParseMessage(myMTK3329,&interuptData[0]);
            newMessage = false;
        }

    }
}

void SERCOM5_Handler() {
    
	uint8_t bytesRead = usartDataRead(SERCOM5,interuptData,255);

    newMessage = true;

}