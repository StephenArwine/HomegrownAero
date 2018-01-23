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


    /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;


    /* Turn on the digital interface clock */
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;


    SystemInit();
    GclkInit();
    RtcInit();
    delayInit();
    adcInit();



    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 0x00);


    pinOut(LedPin);
    pinAnalog(senseBatPin);


    //pinAnalog(senseAPin);
    pinIn(senseAPin);
    pinMux(senseAPin);
    pinOut(fireAPin);
    pinLow(fireAPin);


    //pinAnalog(senseBPin);
    pinIn(senseBPin);
    pinMux(senseBPin);
    pinOut(fireBPin);
    pinLow(fireBPin);


    // pinAnalog(senseCPin);
    pinIn(senseCPin);
    pinMux(senseCPin);
    pinOut(fireCPin);
    pinLow(fireCPin);


    // pinAnalog(senseDPin);
    pinIn(senseDPin);
    pinMux(senseDPin);
    pinOut(fireDPin);
    pinLow(fireDPin);

    charges.Afired = false;
    charges.Bfired = false;
    charges.Cfired = false;
    charges.Dfired = false;



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
    //pinMux(spi2MISO);
    //pinMux(spi2SCK);
    //pinMux(spi2MOSI);


    pinOut(cs_baro);
    pinHigh(cs_baro);

    pinOut(buzzerPin);
    pinCfg(buzzerPin);


    pinOut(TxPo);
    pinMux(TxPo);
    pinIn(RxPo);
    pinMux(RxPo);


    sercomClockEnable(SPI2, 3, 4);
    sercomSpiMasterInit(SPI2, 1, 3, 0, 0, 45403);


    sercomClockEnable(SPI0, 3, 4);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);


    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);


    sercomClockEnable(USART3, 4, 8);
    sercomUartInit(USART3,1,0,19200);


    TC4Init();
    TC5Init();
}




int main(void) {

    init();

    initMS5803Barometer();
    IMUinit();

    computeKalmanGains();


    sampleTick();

    beep(400);


    startupTick = millis();


    while (1) {


        if (takeSample()) {
            sampleTick();
            flight();
            computeKalmanStates();



        }
    }
}
