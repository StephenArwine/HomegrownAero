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

    /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;


    /* Turn on the digital interface clock */
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

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

//     pinLow(cs_tx);
//     while(pinRead(spiMISO) == true);
//     byteOut(spiSCK, spiMOSI, CC1101_SRES);
//     pinHigh(cs_tx);

    // delay_ms(100);

    pinLow(cs_mem);
    byteOut(spiSCK,spiMOSI,0x9f);
    u8_t ID = byteIn(spiSCK, spiMISO);
    u8_t ID2 = byteIn(spiSCK, spiMISO);
    u8_t ID3 = byteIn(spiSCK, spiMISO);
    pinHigh(cs_mem);


    write_cc1101_status_regersters();
    delay_ms(100);

    //sendreg();

    delay_ms(100);


    CC1101_cmd_strobe(CC1101_SFSTXON);




    while (1) {

//         u8_t packet[18] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
//
//         volatile bool sent = CC1101_tx_data(packet, 0x18);
//
//         u8_t status2 = CC1101_read_status_reg(CC1101_MARCSTATE);


        parseGPSMessage();

        if (myMessage.messageReady == true && myMessage.transmitMessage == true) {
            myMessage.messageReady = false;
            myMessage.transmitMessage = false;

            u8_t packet[18] = {0xAA, 0x0A, 0xAA, 0xFA, 0xAA, 0xAA, 0xA2, 0xAA, 0xAA, 0xAA, 0x0A, 0xAA, 0xAA, 0xAA, 0xAA, 0xA7, 0xAA, 0xAA};

            volatile bool sent = CC1101_tx_data(packet, 0x18);

            u8_t status2 = CC1101_read_status_reg(CC1101_MARCSTATE);

            sendUSARTMessage(myMessage);

            char * sencC = sent ? "true" : "false";


            SendUSART(sencC, strlen(sencC));
            //SendUSART(cc1101_reg[CC1101_IOCFG0].addr, strlen(cc1101_reg[CC1101_IOCFG0].addr));
            //write_cc1101_status_regersters();

            //sendreg();



        }
    }



}
