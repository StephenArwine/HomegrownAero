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

void GPSInit(){
	
	    usartDataOut(2, 0x06);

	
	
	//B5 62 06 01 08 00 F0 01 00 00 00 00 00 00 00 2A
	
	
	
	
	
}

void parseGPSMessage() {

    if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {


        u8_t message[255];
        u8_t messageLength = 0;

        for (u8_t i = 0; i < 255; i++) {
            while(sercom(2)->SPI.INTFLAG.bit.RXC == 0);
            u8_t digit = usartDataIn(2);
            if (digit == 0x0D ) {
                break;
            }
            message[i] = digit;
            messageLength++;
        }

        char *msgToParse = message;

        if (message[4] == 0x52 && message[5] == 0x4d) {

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
