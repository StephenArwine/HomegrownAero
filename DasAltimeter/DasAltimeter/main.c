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
    pinAnalog(senseBat);

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);

    pinOut(cs_mem);
    pinHigh(cs_mem);



}

void sendMessage() {


}


int main(void) {
    /* Initialize the SAM system */
    init();



    sercomClockEnable(1, 3, 6);
    sercomSpiMasterInit(1, 0, 2, 0, 0, 0x01);

    dmaSercomConfigureTx(DMA_SERCOM1_TX, 1);
    dmaSercomConfigureRx(DMA_SERCOM1_RX, 1);
    dmaEnableInterrupt(DMA_SERCOM1_RX);
    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);
    pinGpio(cs_mem);


    volatile float batV = 0;


    /* Replace with your application code */
    volatile uint8_t rData;
    volatile uint8_t rData2;
    volatile uint8_t rData3;



    uint8_t sData = 0x9f;
    uint8_t sData2 = 0x0A;


    //  pinHigh(spi1SCK);


    volatile long counter = 0;



    //u32_t txoffset = 0;
    u32_t sizeTX = 100;
//
    //dmaSercomStartRx(DMA_SERCOM1_RX,1,NULL,sizeTX);
    //dmaSercomStartTx(DMA_SERCOM1_TX,1,&sData,sizeTX);
//
//
//
    //int32_t rxOffset = 0;
    u32_t sizeRX = 8;
//
    //dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData,sizeRX);
    //dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);
//
    //dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData2,sizeRX);
    //dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);
//
    //dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData3,sizeRX);
    //dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);

//    byteOut(spi1SCK,spi1MOSI,sData);
//    rData = byteIn(spi1SCK,spi1MISO);
    //rData2 = byteIn(spi1SCK,spi1MISO);
    //  rData3 = byteIn(spi1SCK,spi1MISO);


    pinLow(cs_mem);
    dmaSercomStartTx(DMA_SERCOM1_TX,1,&sData,sizeTX);
    // dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData,sizeRX);
    pinHigh(cs_mem);
    dmaAbort(1);

    void spiSend(uint8_t sdata) {
        while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
        SERCOM1->SPI.DATA.reg = sdata;
        while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
    }


    const u8_t dummy_tx = 0xFF;
    u8_t spiReceive() {
        SERCOM1->SPI.DATA.reg = dummy_tx;
        u8_t RX = SERCOM1->SPI.DATA.reg;
        while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
       while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
        return RX;
    }


    while (1) {
        counter++;
        pinToggle(LedPin);
        batV = (0.0020676 * adc_read(senseBat));

        delay_ms(10);


        //  pinLow(cs_mem);
        //  byteOut(spi1SCK,spi1MOSI,sData);
        //  pinHigh(cs_mem);




        pinLow(cs_mem);

        spiSend(sData);
        rData = spiReceive();
        rData2 = spiReceive();
        rData3 = spiReceive();


        //  dmaSercomStartTx(DMA_SERCOM1_TX,1,&sData,sizeTX);
        //  while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
        //dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData,sizeRX);
        //dmaAbort(1);
        pinHigh(cs_mem);

        flight();


    }
}


