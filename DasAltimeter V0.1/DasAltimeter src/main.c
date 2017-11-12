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
    RtcInit();
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



    pinOut(cs_baro);
    pinHigh(cs_baro);
    pinGpio(cs_baro);

    pinOut(buzzerPin);
    pinCfg(buzzerPin);

//    sercomClockEnable(SPI2, 3, 4);
//   sercomSpiMasterInit(SPI2, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI0, 3, 4);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);

}


int main(void) {

    init();

    Altimeter my_altimeter;

    initMS5803Barometer(&my_altimeter.myBarometer);
    IMUinit();


    u8_t counter = 0;


    volatile float averageAccel;
    volatile float averageAlt;
    volatile float baseAltitude;


    delay_ms(300);

    for (u16_t baseNum = 0; baseNum < 400; ++baseNum) {
        sampleTick(&my_altimeter);

        baseAltitude -= baseAltitude / 50;
        baseAltitude += my_altimeter.myBarometer.heightFeet / 50;
        delay_us(150);

    }

    averageAlt = baseAltitude;

    u32_t time = 0;
    u32_t lastTime = 0;

    while (1) {
        time = millis();
        counter++;
        sampleTick(&my_altimeter);

        averageAlt -= averageAlt / 20;
        averageAlt += my_altimeter.myBarometer.heightFeet / 20;

        averageAccel -= averageAccel / 10;
        averageAccel += my_altimeter.myAnalogAccelerometer.analogAccel / 10;



        /*
        if (abs(averageAlt - baseAltitude) > 20) {
        	for (u16_t buzz = 0; buzz < 200; ++buzz) {
        		delay_us(150);
        		pinToggle(buzzerPin);
        	}
        }
        */

        if ((time - lastTime) > 10000) {
            pinToggle(LedPin);
            lastTime = time;
        }


        if (counter == 100) {
            //  pinToggle(LedPin);

            for (u16_t buzz = 0; buzz < 200; ++buzz) {
                pinToggle(buzzerPin);
                delay_us(50);
            }


            counter = -1;
        }


    }
}


