#include <util.h>
#include <boardDefines.h>


void attemptConnection() {

    if (USARTconnectionAvaliable()) {

        flightState = flightIdle;

        //wait for user to tell us what they want
        while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
        u8_t option = usartDataIn(USART3);

        //user wants to read flight logs
        if (option == 0x4C) {

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
        //user wants to erase chip
        if (option == 0x45) { // 'E'
            AT25SFChipErase();

            AT25SFHoldTillReady();

            usartDataOut(USART3, 'E');
            beep(400);

        }

        if (option == 0x83 ) { // 'S'


        }
    }

}
