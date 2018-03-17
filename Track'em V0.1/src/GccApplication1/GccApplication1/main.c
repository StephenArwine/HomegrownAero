/*
 * GccApplication1.c
 *
 * Created: 3/17/2018 4:59:12 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>
#include <port.h>

const static Pin spiMOSI = {.group = 0, .pin = 23};
const static Pin spiMISO = {.group = 0, .pin = 21};
const static Pin spiSCK = {.group = 0, .pin = 22};

const static Pin cs_tx = {.group = 0, .pin = 16};
const static Pin GDO0 = {.group = 0, .pin = 17};

int main(void) {
    /* Initialize the SAM system */
    SystemInit();



    pinOut(spiMOSI);
    pinOut(spiSCK);
    pinIn(spiMISO);

    pinOut(cs_tx);
    pinHigh(cs_tx);

    pinIn(GDO0);

    pinLow(cs_tx);

    u32_t time = 1;
    time = time + 1;

    /* Replace with your application code */


    while (1) {
    }
}
