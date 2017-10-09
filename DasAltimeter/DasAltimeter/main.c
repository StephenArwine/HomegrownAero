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
    NVIC_SetPriority(DMAC_IRQn, 0xff);

    pinOut(LedPin);
    pinAnalog(senseBat);

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);

    pinOut(cs_mem);
    pinHigh(cs_mem);



}


int main(void) {
    /* Initialize the SAM system */
    init();



    sercomClockEnable(1, 3, 12);
    sercomSpiMasterInit(1, 0, 2, 0, 0, 0x01);

    dmaSercomConfigureTx(DMA_SERCOM1_TX, 1);
    dmaSercomConfigureRx(DMA_SERCOM1_RX, 1);
    dmaEnableInterrupt(DMA_SERCOM1_RX);
    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);


    volatile float batV = 0;


    /* Replace with your application code */
    uint8_t rData;
    uint8_t rData2;
    uint8_t rData3;



    uint8_t sData = 0x9F;


    volatile long counter = 0;

    pinLow(cs_mem);

    u32_t txoffset = 0;
    u32_t sizeTX = 8;

    dmaSercomStartRx(DMA_SERCOM1_RX,1,NULL,sizeTX);
    dmaSercomStartTx(DMA_SERCOM1_TX,1,&sData,sizeTX);



    int32_t rxOffset = 0;
    u32_t sizeRX = 8;

    dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData,sizeRX);
    dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);

    dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData2,sizeRX);
    dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);

    dmaSercomStartRx(DMA_SERCOM1_RX,1,&rData3,sizeRX);
    dmaSercomStartTx(DMA_SERCOM1_TX,1,NULL,sizeRX);


    pinHigh(cs_mem);



    while (1) {
        counter++;
        pinToggle(LedPin);
        //	batV = (0.0020676 * adc_read(senseBat));
        delay_ms(1000);



    }
}


