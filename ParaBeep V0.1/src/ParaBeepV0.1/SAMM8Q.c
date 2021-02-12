#include <SAMM8Q.h>


//Given a message, calc and store the two byte "8-Bit Fletcher" checksum over the entirety of the message
//This is called before we send a command message
void calcChecksum(ubxPacket *msg, uint8_t *payloadCfg) {

    msg->checksumA = 0;
    msg->checksumB = 0;

    msg->checksumA += msg->cls;
    msg->checksumB += msg->checksumA;

    msg->checksumA += msg->id;
    msg->checksumB += msg->checksumA;

    msg->checksumA += (msg->len & 0xFF);
    msg->checksumB += msg->checksumA;

    msg->checksumA += (msg->len >> 8);
    msg->checksumB += msg->checksumA;

    for (uint16_t i = 0; i < msg->len; i++) {
        msg->checksumA += payloadCfg[i];
        msg->checksumB += msg->checksumA;
    }

}

void sendCommand(ubxPacket *outgoingUBX, uint8_t *payloadCfg) {

    calcChecksum(outgoingUBX , payloadCfg); //Sets checksum A and B bytes of the packet

    //Write header bytes
    usartDataOut(0, 0xB5);
    usartDataOut(0, 0x62);
    usartDataOut(0, outgoingUBX->cls);
    usartDataOut(0, outgoingUBX->id);
    usartDataOut(0, outgoingUBX->len & 0xFF);
    usartDataOut(0, outgoingUBX->len >> 8);

    //Write payload.
    for (int i = 0; i < outgoingUBX->len; i++) {
        usartDataOut(0, payloadCfg[i]);
    }

    //Write checksum
    usartDataOut(0, outgoingUBX->checksumA);
    usartDataOut(0, outgoingUBX->checksumB);
    volatile u8_t dump = 1;
}

void powerOffWithInterrupt() {

    ubxPacket packetCfg;

    packetCfg.cls = UBX_CLASS_CFG; // 0x06
    packetCfg.id = UBX_CFG_PM2;  // 0x3B
    packetCfg.len = 7;
    packetCfg.startingSpot = 0;

    usartDataOut(2, 0x06);
    usartDataOut(2, 0x3b);
    usartDataOut(2, 0x07);

    uint8_t payloadCfg[7];
    payloadCfg[0] = 0x02;
    payloadCfg[1] = 0x00;
    payloadCfg[2] = 0x00;
    payloadCfg[3] = 0x00;
    payloadCfg[4] = 0x00;
    payloadCfg[5] = 0x01;
    payloadCfg[6] = 0x01;

    for (int i = 0; i < packetCfg.len; i++) {
        usartDataOut(2, payloadCfg[i]);
    }

    calcChecksum(&packetCfg , &payloadCfg);
    usartDataOut(2, packetCfg.checksumA);
    usartDataOut(2, packetCfg.checksumB);

    //sendCommand(&packetCfg, &packetCfg);

}




inline u8_t findNextComma(char *message) {

    u8_t nextComma = 0;

    for (u8_t i = 0;; i++) {
        nextComma = i;
        if (message[i] == 0x2C) {
            break;
        }
    }
    return nextComma;
}

void GPSInit() {

    u8_t PM2CONFIG[52] = {
        0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x00, 0x00, 0x6E, 0x10, 0x40, 0x01,
        0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x86, 0x02, 0x00, 0x00,
        0xFE, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x0C, 0x2A
    };


    u8_t GLLOFF[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x2A};
    u8_t GSAOFF[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x31};
    u8_t GSVOFF[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x38};
    u8_t VTGOFF[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};


    for (u8_t i = 0 ; i < 52; i++) {
        usartDataOut(2, PM2CONFIG[i]);
    }
    delay_ms(200);
//     for (u8_t i = 0 ; i < 16; i++) {
//         usartDataOut(2, GLLOFF[i]);
//     }
//     delay_ms(200);
//     for (u8_t i = 0 ; i < 16; i++) {
//         usartDataOut(2, GSAOFF[i]);
//     }
//     delay_ms(200);
//     for (u8_t i = 0 ; i < 16; i++) {
//         usartDataOut(2, GSVOFF[i]);
//     }
//     for (u8_t i = 0 ; i < 16; i++) {
//         usartDataOut(2, VTGOFF[i]);
//     }




}

void BYPASSDATAOUT(SercomId id, u8_t data) {
    sercom(id)->USART.DATA.reg = data;
}

u8_t BYPASSDATAIN(SercomId id) {
    return sercom(id)->USART.DATA.reg;
}

void parseGPSMessage() {

    if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {


        u8_t message[600];
        u8_t messageLength = 0;

        for (u8_t i = 0; i < 600; i++) {
            while(sercom(2)->SPI.INTFLAG.bit.RXC == 0);
            u8_t digit = usartDataIn(2);
            if (digit == 0x0A ) {
                break;
            }
            message[i] = digit;
            messageLength++;
        }

        char *msgToParse = message;


        if (message[3] == 0x52 || message[4] == 0x4d) {
            //GNRMC
            u8_t nextComma = findNextComma(message);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.time,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.quality,msgToParse,nextComma);

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
            strncpy(myMessage.speedOverGround,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.courseOverGround,msgToParse,nextComma);

            myMessage.messageReady = true;

        }


        if (message[3] == 0x47 || message[4] == 0x47) {
            //GNGGA
            u8_t nextComma = findNextComma(message);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.time,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.quality,msgToParse,nextComma);

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
            strncpy(myMessage.speedOverGround,msgToParse,nextComma);

            msgToParse = strchr(msgToParse,',') + 1;
            nextComma = findNextComma(msgToParse);
            strncpy(myMessage.courseOverGround,msgToParse,nextComma);

            myMessage.messageReady = true;

        }
    }
}


void GPSUSARTBYPASS() {

    while(1) {

        if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {
            u8_t digit = BYPASSDATAIN(2);
            BYPASSDATAOUT(0, digit);
        }

        if(sercom(0)->USART.INTFLAG.bit.RXC == 1) {
            u8_t digit = BYPASSDATAIN(0);
            BYPASSDATAOUT(2, digit);
        }


    }



}