/*
 * Parabeep V0.2.c
 *
 * Created: 2/6/2021 7:59:13 AM
 * Author : Desktop
 */


#include "sam.h"
#include "util.h"

void init() {
    delayInit();

    pinOut(BuzzerPin);

    pinOut(PeriphEN);
    pinHigh(PeriphEN);
    //pinLow(PeriphEN);

    pinOut(LedPin);
    pinHigh(LedPin);

    pinOut(spi0MOSI);
    pinMux(spi0MOSI);
    pinOut(spi0SCK);
    pinMux(spi0SCK);
    pinOut(cs_accel);
    pinHigh(cs_accel);
    pinOut(cs_baro);
    pinHigh(cs_baro);
    pinIn(spi0MISO);
    pinMux(spi0MISO);

    pinOut(spi5MOSI);
    pinMux(spi5MOSI);
    pinOut(spi5SCK);
    pinMux(spi5SCK);
    pinOut(cs_mem);
    pinHigh(cs_mem);
    pinIn(spi5MISO);
    pinMux(spi5MISO);


    TC2Init();
    SPI0init(0);
    SPI5init(5);

    initMS5803Barometer();

}

void getAlt() {

    pinLow(cs_baro);
    spiDataOut(SPI0, cmdAdcD2_4096_);
    pinHigh(cs_baro);
    delay_us(9040);
    volatile uint32_t tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    spiDataOut(SPI0, cmdAdcD1_4096_);
    pinHigh(cs_baro);

    delay_us(9040);

    volatile u32_t PressureRaw = readMS5803AdcResults();
    ConvertPressureTemperature(PressureRaw, tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);

    sample.pressureAltitude = paToFeetNOAA(sample.pressureMbar);

}


int main(void) {

    init();

    adxlWriteByte(ADXL345_REG_POWER_CTL, 0x00);
    volatile u8_t pwr = adxlReadByte(ADXL345_REG_POWER_CTL);

    adxlWriteByte(ADXL345_REG_DATA_FORMAT, 0x03);
    volatile u8_t data = adxlReadByte(ADXL345_REG_DATA_FORMAT);

    adxlWriteByte(ADXL345_REG_FIFO_CTL, 0x00);
    volatile u8_t fifo = adxlReadByte(ADXL345_REG_FIFO_CTL);

    //adxlWriteByte(ADXL345_REG_BW_RATE, 0x0F);
    volatile u8_t rate = adxlReadByte(ADXL345_REG_BW_RATE);

    adxlWriteByte(ADXL345_REG_POWER_CTL, 0x08);
    volatile u8_t pwr2 = adxlReadByte(ADXL345_REG_POWER_CTL);

    volatile u8_t divid = adxlReadByte(ADXL345_REG_DEVID);
    volatile u8_t pwr4 = adxlReadByte(ADXL345_REG_POWER_CTL);
    volatile u8_t data2 = adxlReadByte(ADXL345_REG_DATA_FORMAT);

    volatile u8_t fifo1 = adxlReadByte(ADXL345_REG_FIFO_STATUS);


    pinLow(cs_mem);
    spiDataOut(SPI5, 0x9f);
    volatile u8_t MEMID0 = spiDataIn(SPI5);
    volatile u8_t MEMID1 = spiDataIn(SPI5);

    pinHigh(cs_mem);
    delay_ms(10);

    //buzzing = true;


    volatile long count = 0;
    while (1) {

        //getAlt();
        adxlWriteByte(ADXL345_REG_POWER_CTL, 0x08);

        volatile u8_t junk = spiDataIn(SPI0);

        pinToggle(LedPin);
        delay_ms(50);

        volatile u8_t divid2 = adxlReadByte(ADXL345_REG_DEVID);


        volatile u8_t x0 = adxlReadByte(ADXL345_REG_DATAX0);
        volatile u8_t x1 = adxlReadByte(ADXL345_REG_DATAX1);
        volatile u8_t y0 = adxlReadByte(ADXL345_REG_DATAY0);
        volatile u8_t y1 = adxlReadByte(ADXL345_REG_DATAY1);
        volatile u8_t z0 = adxlReadByte(ADXL345_REG_DATAZ0);
        volatile u8_t z1 = adxlReadByte(ADXL345_REG_DATAZ1);
        delay_ms(10);
        volatile u8_t junk2 = spiDataIn(SPI0);

        volatile u8_t pwr3 = adxlReadByte(ADXL345_REG_POWER_CTL);
        volatile u8_t pwr4 = adxlReadByte(ADXL345_REG_POWER_CTL);



        delay_ms(500);



    }
}
