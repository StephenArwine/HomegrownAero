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
    GclkInit(0);
    RtcInit();
    delayInit();
    adcInit();

    pinOut(LedPin);
    pinAnalog(senseBatPin);

    pinIn(analogAccelPin);
    pinMux(analogAccelPin);

    pinIn(senseAPin);
    pinMux(senseAPin);
    pinOut(fireAPin);
    pinLow(fireAPin);

    pinIn(senseBPin);
    pinMux(senseBPin);
    pinOut(fireBPin);
    pinLow(fireBPin);

    pinIn(senseCPin);
    pinMux(senseCPin);
    pinOut(fireCPin);
    pinLow(fireCPin);

    pinIn(senseDPin);
    pinMux(senseDPin);
    pinOut(fireDPin);
    pinLow(fireDPin);

    charges.igniterAHot = false;
    charges.igniterBHot = false;
    charges.igniterCHot = false;
    charges.igniterDHot = false;

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
    pinMux(spi2MISO);
    pinMux(spi2SCK);
    pinMux(spi2MOSI);

    pinOut(cs_baro);
    pinHigh(cs_baro);

    pinOut(buzzerPin);
    pinOutStrong(buzzerPin);

    pinOut(TxPo);
    pinMux(TxPo);
    pinIn(RxPo);
    pinMux(RxPo);

    delay_ms(200);

    sercomClockEnable(SPI2, 3, 4);
    sercomSpiMasterInit(SPI2, 1, 3, 0, 0, 0x00);

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

    computeKalmanBaroOnlyGains();

    sampleTick();

    POST();

    isItPointingUp();

    getSettings();

    beep(100);
    delay_ms(500);

    continuityBeep();
    //delay_ms(1000);

    startupTick = millis();

    while (1) {
        if (takeSample()) {
            sampleTick();
            flight();
            igniterTick();
            computeKalmanStates();

        }

    }

}

void HardFault_Handler(void) {
    NVIC_SystemReset();

}