#include <util.h>





void byteOut(Pin SCK_PIN, Pin MOSI_PIN, uint8_t byte) {

	for (uint8_t i = 0; i<8; ++i) {
		pinLow(SCK_PIN);
		if (byte & 0x80) {
			pinHigh(MOSI_PIN);
			} else {
			pinLow(MOSI_PIN);
		}
		pinHigh(SCK_PIN);
		byte <<= 1;
	}

}

uint8_t byteIn(Pin SCK_PIN, Pin MISO_PIN) {
	uint8_t byte = 0x00;

	for (uint8_t i = 0; i<8; ++i) {
		pinLow(SCK_PIN);
		byte = (byte << 1 ) | pinRead(MISO_PIN);
		pinHigh(SCK_PIN);
	}
	return byte;
}