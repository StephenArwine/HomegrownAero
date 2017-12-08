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
    pinIn(RxPo);
    pinMux(RxPo);

//    sercomClockEnable(SPI2, 3, 4);
//   sercomSpiMasterInit(SPI2, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI0, 3, 4);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);

    sercomClockEnable(USART3, 4, 8);
    sercomUartInit(USART3,1,0,62180);

    TC4Init();
    TC5Init();
}

void startUp(Altimeter *my_altimeter) {

    beep(400);

    u32_t startupTime = millis();

    while((millis() - startupTime) < 10000) {
		
		if USARTconnectionAvaliable(){

			//wait for user to tell us what they want
            while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
			u8_t option = usartDataIn(USART3);

			//user wants to read flight logs
			if (option == 0x4C) {

				//send list of avaliable flight logs
                for (u8_t flightLog = 0; flightLog < 11; ++flightLog) {

                    if (isFlightLogged(flightLog)) {

						usartDataOut(USART3, flightLog + 0x30);
                    }
                }
				//done sending flight numbers
                usartDataOut(USART3, 0x04);

				//wait for user to pick which flight to read
                while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
				u8_t flightToRead = usartDataIn(USART3) - 0x30;

				
                u32_t flightStartAddress = getFlightStartAddress(flightToRead);
                u32_t flightEndAddress = FindFlightEndingAddress(flightToRead);

				//inform of page numbers
                u8_t pagesToSend = (flightEndAddress  - flightStartAddress) >> 8
				usartDataOut(USART3, pagesToSend);
				
				//USART out the flights pages
                sendTheasePagesToComputer(flightStartAddress, flightEndAddress);
				
                break;
            }
            
        }
    }
	startupJingle();
}


int main(void) {

    init();

    Altimeter my_altimeter;
    my_altimeter.myFlightState = flightStatrup;
    initMS5803Barometer(&my_altimeter.myBarometer);
    IMUinit();

    my_altimeter.myFlashMemory.pageLocation = 0x00;
    my_altimeter.myFlashMemory.currentAddress = 0x002000;
    my_altimeter.myFlashMemory.endingAddress = 0x005FFF;
    my_altimeter.myFlashMemory.pageReady = false;


    //findFlight(&my_altimeter);

    /* this looks for a USART connection	 */
    startUp(&my_altimeter);



    // AT25SFErace4KBlock(0x000000);
    // AT25SFErace4KBlock(0x001000);
    // AT25SFErace4KBlock(0x002000);
    // AT25SFErace4KBlock(0x003000);
    // AT25SFErace4KBlock(0x004000);
    // AT25SFErace4KBlock(0x005000);




    //u8_t address[3] = {0x00,0x10,0x00}; // test flight one add
    //u8_t address2[3] = {0x00,0x20,0x00}; // test flight two add

    //AT25SFWriteBytes(FLIGHTZEROSTART, 3, address);
    //delay_ms(10);
    //AT25SFWriteBytes(FLIGHTONESTART, 3, address2);
    //delay_ms(10);








    u32_t timeNow = millis();
    while((millis() - timeNow) < 3000) {
        sampleTick(&my_altimeter);
        flight(&my_altimeter);
    }

    logFlight(&my_altimeter);

    beep(300);



    while (1) {

        if (takeSample) {
            sampleTick(&my_altimeter);
            flight(&my_altimeter);
            takeSample = false;
        }

        if (writeLog) {
            writeLog = false;
            logSensors(&my_altimeter);

            if (my_altimeter.myFlashMemory.pageReady) {
                my_altimeter.myFlashMemory.pageReady = false;

                pinToggle(LedPin);

                if (my_altimeter.myFlashMemory.currentAddress <= my_altimeter.myFlashMemory.endingAddress) {

                    //u8_t bytesWritten = AT25SEWritePage(my_altimeter.myFlashMemory.currentAddress,my_altimeter.myFlashMemory.pageToWrite);
                    my_altimeter.myFlashMemory.currentAddress = (my_altimeter.myFlashMemory.currentAddress + 0x100);
                } else {
                    delay_ms(80);
                    pinToggle(LedPin);
                    delay_ms(80);
                    pinToggle(LedPin);
                    delay_ms(80);
                    pinToggle(LedPin);
                    delay_ms(80);
                    pinToggle(LedPin);
                    delay_ms(80);
                }
            }
        }
    }
}
