/*
 * GccApplication1.c
 *
 * Created: 1/5/2018 9:21:53 PM
 * Author : ancar
 */


#include "sam.h"



int main(void) {
    /* Initialize the SAM system */
    SystemInit();

    volatile uint32_t tick = 0;

    /* Replace with your application code */
    while (1) {

        tick++;
		if (tick > 1000)
		{
			  tick = 1;
		}

    }



}
