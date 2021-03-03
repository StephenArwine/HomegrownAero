#include "util.h"

u8_t adxlReadByte(u8_t address) {
    pinLow(cs_accel);
    spiDataOut(SPI0, address | READ_BIT);
    delay_us(1);
    volatile u8_t rx = spiDataIn(SPI0);
    pinHigh(cs_accel);
    return rx;
}

void adxlWriteByte(u8_t address, u8_t data) {
    pinLow(cs_accel);
    spiDataOut(SPI0, address);
    delay_us(1);

    spiDataOut(SPI0, data);
    pinHigh(cs_accel);
}