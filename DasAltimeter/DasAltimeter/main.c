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

    //  pinOut(spi2MOSI);
//   pinOut(spi2SCK);
    //  pinIn(spi2MISO);
    //  pinMux(spi2MISO);
    //  pinMux(spi2SCK);
    //  pinMux(spi2MOSI);


    pinOut(cs_imu);
    pinHigh(cs_imu);
    pinGpio(cs_imu);

    pinOut(cs_baro);
    pinHigh(cs_baro);
    pinGpio(cs_baro);


    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x06);

}


int main(void) {

    init();


    volatile float batV = 0;

    /* Replace with your application code */
    volatile uint8_t rData;
    volatile uint8_t rData2;
    volatile uint8_t rData3;




    volatile long counter = 0;

    Barometer myBarometer;

    //readMS5803Coefficients(myBarometer);

    uint8_t dummyTx = 0xFF;
    uint8_t dummyRx;

    pinLow(cs_baro);
    dummyRx = spiDataTransfer(SPI1,0x1E);
    pinHigh(cs_baro);

    while (1) {

        counter++;
        pinToggle(LedPin);
        batV = (0.0020676 * adc_read(senseBat));

        delay_ms(5);

        uint8_t convD1 = 0x40;
        uint8_t ADCRead = 0x00;

        uint8_t sData = 0x40;

        pinLow(cs_imu);

        dummyRx = spiDataTransfer(SPI1,0x40 | 0x41);
		delay_us(10);
        rData = spiDataTransfer(SPI1,dummyTx);
   //     rData2 = spiDataTransfer(SPI1,dummyTx);

        //    byteOut(spi1SCK,spi1MOSI,sData);
        //     rData = byteIn(spi1SCK,spi1MISO);
        //    rData2 = byteIn(spi1SCK,spi1MISO);



        pinHigh(cs_imu);


        flight();
    }
}


