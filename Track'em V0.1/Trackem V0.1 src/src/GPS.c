/*
 * GPS.c
 *
 * Created: 1/7/2018 12:19:13 PM
 *  Author: ancar
 */

#include <util.h>
#include <boardDefines.h>

u8_t findNextComma(char *message) {

    u8_t nextComma = 0;

    for (u8_t i = 0;; i++) {
        nextComma = i;
        if (message[i] == 0x2C) {
            break;
        }
    }
    return nextComma;
}


void sendUSARTMessage(messageToSend myMessage) {

    usartDataOut(USART1,0xA);
    for (u8_t i = 0; i < 9; i++) {
        usartDataOut(USART1,myMessage.time[i]);
    }
    usartDataOut(USART1,0x2C);


    for (u8_t i = 0; i < 10; i++) {
        usartDataOut(USART1,myMessage.lat[i]);
    }
    usartDataOut(USART1,0x2C);

    usartDataOut(USART1,myMessage.northSouth[0]);
    usartDataOut(USART1,0x2C);

    for (u8_t i = 0; i < 11; i++) {
        usartDataOut(USART1,myMessage.longitude[i]);
    }
    usartDataOut(USART1,0x2C);

    usartDataOut(USART1,myMessage.eastWest[0]);
    usartDataOut(USART1,0x2C);

    for (u8_t i = 0; i < 10; i++) {
        if (myMessage.altitude[i] == 0) {
            break;
        }
        usartDataOut(USART1,myMessage.altitude[i]);
    }
    usartDataOut(USART1,0x2C);

    for (u8_t i = 0; i < 10; i++) {
        if (myMessage.speedOverGround[i] == 0) {
            break;
        }
        usartDataOut(USART1,myMessage.speedOverGround[i]);
    }
    usartDataOut(USART1,0x2C);

    for (u8_t i = 0; i < 10; i++) {
        if (myMessage.courseOverGround[i] == 0) {
            break;
        }
        usartDataOut(USART1,myMessage.courseOverGround[i]);
    }
    usartDataOut(USART1,0x2C);


}

void parseGPSMessage() {

    if(sercom(USART0)->SPI.INTFLAG.bit.RXC == 1) {

        u8_t message[255];
        u8_t messageLength = 0;

        for (u8_t i = 0; i < 255; i++) {
            while(sercom(USART0)->SPI.INTFLAG.bit.RXC == 0);
            u8_t digit = usartDataIn(USART0);
            if (digit == 0x0D ) {
                break;
            }
            message[i] = digit;
            messageLength++;
        }

        /*
        for (u8_t i = 0; i < messageLength; i++) {
            usartDataOut(USART1,message[i]);
        }
        usartDataOut(USART1,0x2C);
        */

        char *msgToParse = message;

        if (message[4] == 0x47 && message[5] == 0x47) {

            u8_t nextComma = findNextComma(&message);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.time,msgToParse,nextComma);


            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.lat,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.northSouth,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.longitude,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.eastWest,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.quality,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.numSV,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.HDOP,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.altitude,msgToParse,nextComma);

            myMessage.messageReady = true;


        } else if (message[4] == 0x52 && message[5] == 0x4d) {

            myMessage.messageReady = false;

            u8_t nextComma = findNextComma(&message);

            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.speedOverGround,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.courseOverGround,msgToParse,nextComma);

        }
    }
}