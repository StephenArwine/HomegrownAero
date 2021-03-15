#include "util.h"

u8_t adxlReadByte(u8_t address) {
    pinLow(cs_accel);
    spiDataOut(SPI0, address | READ_BIT);
    delay_us(1);
    u8_t rx = spiDataIn(SPI0);
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

void ADXLgetAccelData() {

    pinLow(cs_accel);
    spiDataOut(SPI0, ADXL345_REG_DATAX0 | READ_BIT | MULTI_BIT);
    sample.ADXLX0 = spiDataIn(SPI0);
    sample.ADXLX1 = spiDataIn(SPI0);
    sample.ADXLY0 = spiDataIn(SPI0);
    sample.ADXLY1 = spiDataIn(SPI0);
    sample.ADXLZ0 = spiDataIn(SPI0);
    sample.ADXLZ1 = spiDataIn(SPI0);
    pinHigh(cs_accel);

}