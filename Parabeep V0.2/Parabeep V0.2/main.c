/*
 * Parabeep V0.2.c
 *
 * Created: 2/6/2021 7:59:13 AM
 * Author : Desktop
 */


#include "sam.h"
#include "util.h"


int main(void) {

    //delayInit();

    pinOut(LedPin);

    pinHigh(LedPin);

    //SPIinit(2);

	volatile long count = 0;
    while (1) {
		++count;
		
		if (count > 100000)
		{
			count = 0;
		}
        //delay_ms(100);
        //pinToggle(LedPin);
    }
}
