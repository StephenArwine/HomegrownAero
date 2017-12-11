#include <util.h>
#include <boardDefines.h>




void attemptConnection(Altimeter *my_altimeter){

        if (USARTconnectionAvaliable()) {

            my_altimeter->myFlightState = flightTest;

            //wait for user to tell us what they want
            while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
            u8_t option = usartDataIn(USART3);

            //user wants to read flight logs
            if (option == 0x4C) {

                //send list of available flight logs
                for (u8_t flightLog = 0; flightLog < 11; ++flightLog) {

                    if (isFlightLogged(flightLog)) {

                        usartDataOut(USART3, flightLog + 0x30);
                    }
                }
                //done sending flight numbers
                usartDataOut(USART3, 0x0F);

                //wait for user to pick which flight to read
                while(sercom(USART3)->SPI.INTFLAG.bit.RXC == 0);
                u8_t flightToRead = usartDataIn(USART3) - 0x30;


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

                break;
            }
            //user wants to erase chip
            if (option == 0x45) { // 'E'
                AT25SFChipErase();

                usartDataOut(USART3, 'E');
                break;
            }

            if (option == 0x83 ) { // 'S'


            }
        }

}
