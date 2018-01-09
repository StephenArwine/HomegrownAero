/*
 * GccApplication1.c
 *
 * Created: 1/5/2018 9:21:53 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>
#include <boardDefines.h>

void init() {

    SystemInit();
    GclkInit();
    RtcInit();
    delayInit();

    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 0x00);

    pinOut(GPSTxPo);
    pinIn(GPSRxPo);
    pinMux(GPSTxPo);
    pinMux(GPSRxPo);

    pinOut(ComTxPo);
    pinIn(ComRxPo);
    pinMux(ComTxPo);
    pinMux(ComRxPo);

    pinOut(spiMOSI);
    pinOut(spiSCK);
    pinIn(spiMISO);

    pinOut(cs_mem);
    pinHigh(cs_mem);
    pinOut(cs_tx);
    pinHigh(cs_tx);

    sercomClockEnable(USART0, 4, 8);
    sercomUartInit(USART0,1,0,63860);

    sercomClockEnable(USART1, 4, 8);
    sercomUartInit(USART1,3,1,45403);

    TC1Init();

}


void SendUSART(char message[], int length) {

    for (u8_t i = 0; i < length; i++) {
        usartDataOut(USART1,message[i]);
    }

}


int main(void) {
    /* Initialize the SAM system */
    init();


    /* Replace with your application code */

    pinLow(cs_mem);
    byteOut(spiSCK,spiMOSI,0x9f);
    volatile u8_t ID = byteIn(spiSCK, spiMISO);
    volatile u8_t ID2 = byteIn(spiSCK, spiMISO);
    volatile u8_t ID3 = byteIn(spiSCK, spiMISO);
    pinHigh(cs_mem);

    pinLow(cs_tx);
    volatile u8_t status = syncByte(spiSCK, spiMISO, spiMOSI, 0x3d);
    volatile u8_t status2 = byteIn(spiSCK, spiMISO);
    pinHigh(cs_tx);


    while (1) {

        parseGPSMessage();

        if (myMessage.messageReady == true && myMessage.transmitMessage == true) {
            myMessage.messageReady = false;
            myMessage.transmitMessage = false;

            sendUSARTMessage(myMessage);

            char message1[] = "message,";

            SendUSART(message1, strlen(message1));
            //SendUSART(cc1101_reg[CC1101_IOCFG0].addr, strlen(cc1101_reg[CC1101_IOCFG0].addr));
            sendreg();
        }
    }



}
