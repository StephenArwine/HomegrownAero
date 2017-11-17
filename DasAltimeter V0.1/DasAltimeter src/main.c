/*
 * DasAltimeter.c
 *
 * Created: 10/4/2017 5:28:21 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>
#include <boardDefines.h>
#include "math.h"




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
    pinMux(senseAPin);
    pinOut(fireAPin);
    pinLow(fireAPin);

    pinAnalog(senseBPin);
    pinMux(senseBPin);
    pinOut(fireBPin);
    pinLow(fireBPin);

    pinAnalog(senseCPin);
    pinOut(fireCPin);
    pinLow(fireCPin);

    pinAnalog(senseDPin);
    pinOut(fireDPin);
    pinLow(fireDPin);

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

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);
    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);

    pinOut(cs_mem);
    pinHigh(cs_mem);



    pinOut(spi2MOSI);
    pinOut(spi2SCK);
    pinIn(spi2MISO);
    //  pinMux(spi2MISO);
    //  pinMux(spi2SCK);
    //  pinMux(spi2MOSI);

    pinOut(cs_baro);
    pinHigh(cs_baro);



    pinOut(buzzerPin);
    pinCfg(buzzerPin);

    pinOut(TxPo);
    pinMux(TxPo);

//    sercomClockEnable(SPI2, 3, 4);
//   sercomSpiMasterInit(SPI2, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI0, 3, 4);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SERCOM3, 4, 8);
    sercomUartInit(SERCOM3,1,0,63858);

    TC4Init();
    TC5Init();
}



int main(void) {

    init();

    Altimeter my_altimeter;
    my_altimeter.myFlightState = flightStatrup;

    initMS5803Barometer(&my_altimeter.myBarometer);
    IMUinit();


    u8_t counter = 0;


    volatile float averageXAccel;
    volatile float averageYAccel;
    volatile float averageZAccel;

    volatile float averageAccel;
    volatile float averageAlt;
    volatile float baseAltitude;



    for (u16_t baseNum = 0; baseNum < 400; ++baseNum) {
        sampleTick(&my_altimeter);

        baseAltitude -= baseAltitude / 50;
        baseAltitude += my_altimeter.myBarometer.heightFeet / 50;
        delay_us(150);

    }

    averageAlt = baseAltitude;

    u32_t time = 0;
    u32_t lastTime = 0;

    //AT25SFErace4KBlock(0);

    my_altimeter.currentAddress = 0x00;



    while (1) {
        time = millis();



        if (takeSample) {
            sampleTick(&my_altimeter);
            flight(&my_altimeter);
            takeSample = false;
            //pinToggle(TxPo);
            usartDataOut(SERCOM3,0x68);
            usartDataOut(SERCOM3,0x65);
            usartDataOut(SERCOM3,0x6c);
            usartDataOut(SERCOM3,0x6c);
            usartDataOut(SERCOM3,0x6f);
            usartDataOut(SERCOM3,0x21);







        }

        if (writeSample) {
            //AT25SFWriteByte(my_altimeter.currentAddress,my_altimeter.myBarometer);
            //volatile u8_t byte1 = AT25SFGetByte(my_altimeter.currentAddress);
            my_altimeter.currentAddress++;
        }





        averageAlt -= averageAlt / 20;
        averageAlt += my_altimeter.myBarometer.heightFeet / 20;


        averageAccel -= averageAccel / 10;
        averageAccel += my_altimeter.myAnalogAccelerometer.analogAccel / 10;

        averageXAccel -= averageXAccel / 3;
        averageXAccel += my_altimeter.myIMU.accelX / 3;

        averageYAccel -= averageYAccel / 3;
        averageYAccel += my_altimeter.myIMU.accelY / 3;

        averageZAccel -= averageZAccel / 3;
        averageZAccel += my_altimeter.myIMU.accelZ / 3;



        volatile float sumAccel = (averageXAccel*averageXAccel + averageYAccel*averageYAccel + averageZAccel*averageZAccel);


        if (sumAccel > 1.3) {
            for (u16_t buzz = 0; buzz < 500; ++buzz) {
                pinToggle(buzzerPin);
                delay_us(122);
            }
        }



    }
}


