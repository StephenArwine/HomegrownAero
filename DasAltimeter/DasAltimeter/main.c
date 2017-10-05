/*
 * DasAltimeter.c
 *
 * Created: 10/4/2017 5:28:21 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    GclkInit();
    delayInit();
    adcInit();

    const static Pin LedPin = {.group = 0, .pin = 27 };
    const static Pin senseBat = {.group = 0, .pin = 6, .chan = 6 };
    pinOut(LedPin);
    pinAnalog(senseBat);
    //pinHigh(LedPin);
	
	
   volatile float batV = 0;
	
	
    /* Replace with your application code */
    volatile long counter = 0;
    while (1) {
        counter++;
        pinToggle(LedPin);
		batV = (0.0020676 * adc_read(senseBat));
        delay_ms(1000);
    }
}
