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

    pinOut(spi2MOSI);
    pinOut(spi2SCK);
    pinIn(spi2MISO);

    pinOut(cs_mem);
    pinHigh(cs_mem);

    pinOut(cs_baro);
    pinHigh(cs_baro);




}


void spiSend(uint8_t sdata) {

    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = sdata;
    while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);

}


u8_t spiReceiveWait() {
    u8_t dummy_tx = 0xFF;
    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = dummy_tx;
    while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
    u8_t RX = SERCOM1->SPI.DATA.reg;

    return RX;
}

u8_t spiReceive() {
    u8_t dummy_tx = 0xFF;
    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = dummy_tx;
    u8_t RX = SERCOM1->SPI.DATA.reg;

    return RX;
}

int main(void) {
    /* Initialize the SAM system */
    init();



    sercomClockEnable(1, 3, 6);
    sercomSpiMasterInit(1, 0, 2, 0, 0, 0x01);


    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);
    pinGpio(cs_mem);


    volatile float batV = 0;


    /* Replace with your application code */
    volatile uint8_t rData;
    volatile uint8_t rData2;
    volatile uint8_t rData3;
    volatile u8_t rDataFake;



    uint8_t sData = 0x9f;
    uint8_t sData2 = 0x0A;




    volatile long counter = 0;

    Barometer myBarometer;



    u32_t sizeRX = 8;


    readMS5803Coefficients(myBarometer);


    while (1) {

        counter++;
        pinToggle(LedPin);
        batV = (0.0020676 * adc_read(senseBat));

        delay_ms(10);


        /*
                pinLow(cs_mem);

                spiSend(sData);
                rData = spiReceive();
                rData2 = spiReceive();
                rData3 = spiReceive();
                rDataFake = spiReceive();
                while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
                while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
                pinHigh(cs_mem);
        */


        flight();


    }
}


