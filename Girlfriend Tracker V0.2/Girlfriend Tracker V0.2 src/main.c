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
    GclkInit(0);
    RtcInit();
    delayInit();

    //NVIC_EnableIRQ(DMAC_IRQn);
    //NVIC_SetPriority(DMAC_IRQn, 0x00);

    pinOut(GPSTxPo);
    pinIn(GPSRxPo);
    pinMux(GPSTxPo);
    pinMux(GPSRxPo);

    pinOut(LEDPin);
    pinOut(AMP_EN);
    pinIn(Pin_GDO0);

//     pinOut(ComTxPo);
//     pinIn(ComRxPo);
//     pinMux(ComTxPo);
//     pinMux(ComRxPo);

    pinOut(spiMOSI);
    pinOut(spiSCK);
    pinIn(spiMISO);

    //pinOut(cs_mem);
    // pinHigh(cs_mem);
    pinOut(cs_tx);
    pinHigh(cs_tx);

    sercomClockEnable(USART1, 4, 8);
    sercomUartInit(USART1,1,0,63860);

//
//     sercomClockEnable(USART1, 4, 8);
//     sercomUartInit(USART1,3,1,45403);

    //TC1Init();

}


// void SendUSART(char message[], int length) {
//
//     for (u8_t i = 0; i < length; i++) {
//         usartDataOut(USART1,message[i]);
//     }
//
// }

u8_t packet[30] = {0x13, 0x0d, 0x89, 0x0a, 0x1c, 0xdb, 0xae, 0x32, 0x20, 0x9a, 0x50, 0xee, 0x40, 0x78, 0x36, 0xfd, 0x12, 0x49, 0x32, 0xf6, 0x9e, 0x7d, 0x49, 0xdc, 0xad, 0x4f, 0x14, 0xf2 };


int main(void) {
    /* Initialize the SAM system */
    init();


    /* Replace with your application code */


//     pinLow(cs_mem);
//     byteOut(spiSCK,spiMOSI,0x9f);
//     u8_t ID = byteIn(spiSCK, spiMISO);
//     u8_t ID2 = byteIn(spiSCK, spiMISO);
//     u8_t ID3 = byteIn(spiSCK, spiMISO);
//     pinHigh(cs_mem);


    CC1101_reset_chip();
    delay_ms(1000);

    //sendreg();


    CC1101_cmd_strobe(CC1101_SFSTXON);


    cc1101_write_reg(CC1101_PATABLE, 0xC0);

    CC1101_cmd_strobe(CC1101_SIDLE);

    //pinHigh(AMP_EN);



    while (1) {

        delay_ms(1000);
//         //TX packed over VHF

        pinHigh(AMP_EN);
        pinHigh(LEDPin);
        delay_ms(200);

        //volatile bool sent = CC1101_tx_data(packet, 0x1E);

        cc1101_write_reg(CC1101_TXFIFO, 31);
        CC1101_write_burst_reg(CC1101_TXFIFO, packet, 30);

        CC1101_cmd_strobe(CC1101_SIDLE);
        CC1101_cmd_strobe(CC1101_STX);

        volatile u8_t status2 = CC1101_read_status_reg(CC1101_MARCSTATE);

        pinLow(AMP_EN);
        delay_ms(50);
        pinLow(LEDPin);
//
//         //send result of TX over USART
//         char * sencC = sent ? "true" : "false";
//         SendUSART(sencC, strlen(sencC));
//


        //parseGPSMessage();

        //myMessage.messageReady == true && myMessage.transmitMessage == true
        if (myMessage.messageReady == true) {
            //pinToggle(LEDPin);
            myMessage.messageReady = false;
            myMessage.transmitMessage = false;

            //Send the parsed GPS message over USART
            //sendUSARTMessage(myMessage);

            //TX packed over VHF
            //volatile bool sent = CC1101_tx_data(packet, 0x1E);
            //u8_t status2 = CC1101_read_status_reg(CC1101_MARCSTATE);

            //send result of TX over USART
            //char * sencC = sent ? "true" : "false";
            //SendUSART(sencC, strlen(sencC));

        }
    }



}
