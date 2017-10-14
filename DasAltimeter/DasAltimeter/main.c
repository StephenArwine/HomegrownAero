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
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);

}


int main(void) {

    init();


    volatile float batV = 0;

    /* Replace with your application code */

    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    volatile uint8_t rData;
    volatile uint8_t rData2;
    volatile uint8_t rData3;
    volatile uint8_t rData4;
    volatile long counter = 0;

    Barometer my_barometer;

    initMS5803Barometer(&my_barometer);

    volatile int64_t sumAltitude;
    volatile int64_t averageAlt;

    while (1) {

        counter++;
        pinToggle(LedPin);
        batV = (0.0020676 * adc_read(senseBat));

        pinLow(cs_baro);
        dummy_rx = spiDataTransfer(SPI1, 0x50);
        pinHigh(cs_baro);
        delay_ms(2);
        my_barometer.rawTempatureData = readMS5803AdcResults();

        pinLow(cs_baro);
        dummy_rx = spiDataTransfer(SPI1, 0x42);
        pinHigh(cs_baro);
        delay_ms(2);
        my_barometer.rawPressureData = readMS5803AdcResults();
        flight();

        ConvertPressureTemperature(&my_barometer);
        pascalToCent(&my_barometer);
        my_barometer.heightFeet = 0.03281 *my_barometer.heightCm;


        sumAltitude = sumAltitude + my_barometer.heightFeet;

        if (counter > 99) {
            averageAlt = sumAltitude / 100;
			sumAltitude = 0;
            counter = 0;

        }


    }
}


