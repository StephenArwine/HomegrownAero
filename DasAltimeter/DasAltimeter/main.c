/*
 * DasAltimeter.c
 *
 * Created: 10/4/2017 5:28:21 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>

uint8_t spiSend(uint8_t data) {
	while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
	SERCOM1->SPI.DATA.reg = data;
	while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
	return SERCOM1->SPI.DATA.reg;
}


int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    GclkInit();
    delayInit();
    adcInit();
    dmaInit();

    const static Pin LedPin = {.group = 0, .pin = 27 };
    const static Pin senseBat = {.group = 0, .pin = 6, .chan = 6 };
    pinOut(LedPin);
    pinAnalog(senseBat);
    //pinHigh(LedPin);

    const static Pin cs_mem = {.group = 0, .pin = 20 };
    const static Pin spi1MISO = {.group = 0, .pin = 16, .mux = MUX_PA16C_SERCOM1_PAD0 };
    const static Pin spi1SCK = {.group = 0, .pin = 17, .mux = MUX_PA17C_SERCOM1_PAD1 };
    const static Pin spi1MOSI = {.group = 0, .pin = 19, .mux = MUX_PA19C_SERCOM1_PAD3 };
    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);
    pinOut(cs_mem);
	
	pinMux(spi1MISO);
	pinMux(spi1SCK);
	pinMux(spi1MOSI);

    sercomClockEnable(1, 3, 12);
    sercomSpiMasterInit(1, 0, 2, 0, 0, 9800);



    volatile float batV = 0;


    /* Replace with your application code */
	volatile uint8_t rData;
	volatile uint8_t sData = 0x9F;
	
	
    volatile long counter = 0;
    while (1) {
        counter++;
        pinToggle(LedPin);
        //	batV = (0.0020676 * adc_read(senseBat));
        delay_ms(1000);
		pinLow(cs_mem);
		rData = spiSend(sData);
		pinHigh(cs_mem);
    }
}


