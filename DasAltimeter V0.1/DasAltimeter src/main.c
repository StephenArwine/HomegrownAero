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


    pinOut(spi0MOSI);
    pinOut(spi0SCK);
    pinIn(spi0MISO);
    pinMux(spi0MISO);
    pinMux(spi0SCK);
    pinMux(spi0MOSI);

    pinOut(cs_accel);
    pinHigh(cs_accel);

    pinOut(cs_gyro);
    pinHigh(cs_gyro);

    pinOut(spi2MOSI);
    pinOut(spi2SCK);
    pinIn(spi2MISO);
    //  pinMux(spi2MISO);
    //  pinMux(spi2SCK);
    //  pinMux(spi2MOSI);


    //  pinOut(cs_imu);
    //  pinHigh(cs_imu);
    //  pinGpio(cs_imu);

    pinOut(cs_baro);
    pinHigh(cs_baro);
    pinGpio(cs_baro);

    pinOut(buzzerPin);
    pinCfg(buzzerPin);

//    sercomClockEnable(SPI2, 3, 4);
//   sercomSpiMasterInit(SPI2, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI0, 3, 32);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);

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




    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, 0x80 | 0x0F);
    volatile u8_t _byte1 = spiDataTransfer(SPI0,dummy_Tx);
    pinHigh(cs_accel);

    delay_ms(300);

    volatile int16_t accelZ;
	


    while (1) {
        counter++;
        sampleTick(&my_altimeter);





        pinLow(cs_accel);
        dummy_rx = spiDataTransfer(SPI0, 0x80 | 0x06);
        volatile u8_t _byte1 = spiDataTransfer(SPI0,dummy_Tx);
        volatile u8_t _byte2 = spiDataTransfer(SPI0,dummy_Tx);
        pinHigh(cs_accel);

        bool negativeZ = (_byte2 & (1<<7)) != 0;

        accelZ = (_byte2 << 8) | _byte1;

        if (_byte2 != 1) {
            if (negativeZ) {
				
                accelZ = accelZ | ~((1 << 16) - 1);
                accelZ = accelZ >> 4;

            } else {
                accelZ = accelZ >> 4;
            }
        }

        volatile float accelZF = accelZ * .000976;



        analogSample = adc_read(analogAccelPin);
        accelX = (analogSample - 3878) * -0.0227;

        averageAccel = averageAccel + accelX;
        averageAlt = averageAlt + my_altimeter.myBarometer.heightFeet;

        if (counter == 100) {
            pinToggle(LedPin);

            averageAccel = averageAccel / 100;
            averageAlt = averageAlt / 100;
            counter = -1;

            averageAccel = 0;
            averageAlt = 0;
        }


    }
}


