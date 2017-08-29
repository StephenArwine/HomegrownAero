#include <util.h>


uint8_t syncByte(Pin SCK_PIN,
                 Pin MOSI_PIN,
                 Pin MISO_PIN,
                 uint8_t byte) {

    uint8_t byteIn = 0x00;

    for (uint8_t i = 0; i<8; ++i) {
        pinLow(SCK_PIN);
        if (byte & 0x80) {
            pinHigh(MOSI_PIN);
        } else {
            pinLow(MOSI_PIN);
        }
        byteIn = (byteIn << 1 ) | pinRead(MISO_PIN);
        pinHigh(SCK_PIN);
    }
	return byteIn;
}

void byteOut(uint8_t SCK_PIN,
             uint8_t MOSI_PIN,
             uint8_t byte) {

    for (uint8_t i = 0; i<8; ++i) {
        SaLDigitalOut(SCK_PIN,FALSE);
        if (byte & 0x80) {
            SaLDigitalOut(MOSI_PIN,true);
        } else {
            SaLDigitalOut(MOSI_PIN,false);
        }
        SaLDigitalOut(SCK_PIN,TRUE);
        byte <<= 1;
    }

}

uint8_t getByte(uint8_t SCK_PIN,
                uint8_t MISO_PIN) {
    uint8_t byte = 0x00;

    for (uint8_t i = 0; i<8; ++i) {
        SaLDigitalOut(SCK_PIN,FALSE);
        byte = (byte << 1 ) | SaLDigitalIn(MISO_PIN);
        SaLDigitalOut(SCK_PIN,TRUE);
    }
    return byte;
}

