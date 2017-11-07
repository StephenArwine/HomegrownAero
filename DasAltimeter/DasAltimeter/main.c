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




void init() {
    SystemInit();
    GclkInit();
    delayInit();
    adcInit();
    dmaInit();

    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 0x00);

    pinOut(LedPin);
    pinAnalog(senseBatPin);
	
    pinAnalog(senseAPin);
    pinOut(fireAPin);
	pinLow(fireAPin);


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

    pinOut(buzzerPin);
    pinCfg(buzzerPin);

    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);

}


int main(void) {

    init();

    Altimeter my_altimeter;

    initMS5803Barometer(&my_altimeter.myBarometer);


    volatile long counter = 0;

    volatile float sumAccel;
    volatile float averageAccel;
    volatile float averageAlt;

    volatile u16_t analogSample;
    volatile float accelX;

volatile u16_t ignighterA;

    while (1) {

    ignighterA = adc_read(senseAPin);



        counter++;
//       pinToggle(LedPin);


           sampleTick(&my_altimeter);

        pinToggle(buzzerPin);
        delay_ms(1);

        uint8_t dummy_Tx = 0xFF;
        uint8_t dummy_rx;

        analogSample = adc_read(analogAccelPin);
        accelX = (analogSample - 3920) * 0.0227;

        averageAccel = averageAccel + accelX;
        averageAlt = averageAlt + my_altimeter.myBarometer.heightFeet;

        if (counter == 100) {
            pinToggle(LedPin);

            averageAlt = averageAlt / 100;
            averageAccel = averageAccel / 100;

            counter = -1;

            averageAccel = 0;
            averageAlt = 0;
        }


    }
}


