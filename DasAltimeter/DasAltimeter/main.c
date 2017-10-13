/*
 * DasAltimeter.c
 *
 * Created: 10/4/2017 5:28:21 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>
#include <flight.h>
#include <boardDefines.h>
#include <MS5803.h>



void init() {
    SystemInit();
    GclkInit();
    delayInit();
    adcInit();
    dmaInit();

    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 0x00);

    pinOut(LedPin);
    pinAnalog(senseBat);

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);
   pinMux(spi1MISO);
   pinMux(spi1SCK);
   pinMux(spi1MOSI);

    pinOut(spi2MOSI);
    pinOut(spi2SCK);
    pinIn(spi2MISO);
   pinMux(spi2MISO);
   pinMux(spi2SCK);
   pinMux(spi2MOSI);
 

    pinOut(cs_mem);
    pinHigh(cs_mem);
pinGpio(cs_mem);

    pinOut(cs_baro);
    pinHigh(cs_baro);




}


int main(void) {
    /* Initialize the SAM system */
    init();

    sercomClockEnable(1, 3, 4);
    sercomSpiMasterInit(1, 0, 2, 0, 0, 0x00);

    volatile float batV = 0;


    /* Replace with your application code */
    volatile uint8_t rData;
    volatile uint8_t rData2;
    volatile uint8_t rData3;
    volatile u8_t rDataFake;

    uint8_t sData = 0x9f;


    volatile long counter = 0;

    Barometer myBarometer;



    u32_t sizeRX = 8;


    readMS5803Coefficients(myBarometer);

    uint8_t dummyTx = 0xFF;
    uint8_t dummyRx;

    while (1) {

        counter++;
        pinToggle(LedPin);
        batV = (0.0020676 * adc_read(senseBat));

        delay_ms(10);



        pinLow(cs_mem);
              dummyRx = spiDataTransfer(sData);
              rData = spiDataTransfer(dummyTx);
              rData2 = spiDataTransfer(dummyTx);
              rData3 = spiDataTransfer(dummyTx);

        pinHigh(cs_mem);


        flight();


    }
}


