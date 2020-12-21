#include <util.h>
#include <boardDefines.h>

void sendFlightLogs() {

    //send list of available flight logs
    for (u8_t flightLog = 0; flightLog < 20; ++flightLog) {

        if (isFlightLogged(flightLog)) {

            usartDataOut(USART3, flightLog);
        }
    }
    //done sending flight numbers
    usartDataOut(USART3, 0xFF);

    //wait for user to pick which flight to read
    while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
    volatile u8_t flightToRead = usartDataIn(USART3);


    u32_t flightStartAddress = getFlightStartAddress(flightToRead);
    u32_t flightEndAddress = FindFlightEndingAddress(flightToRead);

    //inform of page numbers
    u16_t pagesToSend = (flightEndAddress  - flightStartAddress) >> 8;
    usartDataOut(USART3, pagesToSend >> 0);
    usartDataOut(USART3, pagesToSend >> 8);

    //send flight end address for % full
    usartDataOut(USART3, flightEndAddress >> 0);
    usartDataOut(USART3, flightEndAddress >> 8);
    usartDataOut(USART3, flightEndAddress >> 16);

    //USART out the flights pages
    sendTheasePagesToComputer(flightStartAddress, flightEndAddress);

}

void streamSensorData() {
    usartDataOut(USART3, 'R');
    while(1) {
        //wait for comp to request datapoint
        while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
        volatile u8_t compRequest = usartDataIn(USART3);
        if (compRequest == 0x44) {
            sampleTick();
            usartDataOut(USART3, 'A');
			u8_t alt0 = (uint32_t)(sample.pressureAltitude) >> 0;
			u8_t alt1 = (uint32_t)(sample.pressureAltitude) >> 8;
			u8_t alt2 = (uint32_t)(sample.pressureAltitude) >> 16;
			u8_t alt3 = (uint32_t)(sample.pressureAltitude) >> 24;
            usartDataOut(USART3, alt0);
            usartDataOut(USART3, alt1);
            usartDataOut(USART3, alt2);
            usartDataOut(USART3, alt3);
        }
		else if (compRequest == 0x46)
		{
			finishFromPad();
		}
        if (unplugged()) {
            finishFromPad();
        }
    }
}

void startupConnetion() {

    flightState = flightIdle;

    //wait for user to tell us what they want
    while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
    u8_t option = usartDataIn(USART3);

    //user wants to read flight logs
    if (option == 0x4C) {

        sendFlightLogs();
    }
    //user wants to erase chip
    if (option == 0x45) { // 'E'

        //getSettings();

        AT25SFChipErase();
        AT25SFHoldTillReady();
        delay_ms(100);
        AT25SFChipErase();

        AT25SFHoldTillReady();

        //getSettings();

        usartDataOut(USART3, 'E');
        beep(400);

    }

    if (option == 0x53 ) { // 'S'
        delay_ms(1);
        streamSensorData();

    }

}

void attemptConnection() {

    if (USARTconnectionAvaliable()) {

        if (flightState == flightStatrup) {
            startupConnetion();
        }


    }

}
