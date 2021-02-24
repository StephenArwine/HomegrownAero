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
    pinLow(PeriphEN);

    pinOut(LedPin);
    pinHigh(LedPin);

    pinOut(spi0MOSI);
    pinMux(spi0MOSI);
    pinOut(spi0SCK);
    pinMux(spi0SCK);
    //pinOut(cs_accel);
    //pinHigh(cs_accel);
    //pinOut(cs_baro);
    //pinHigh(cs_baro);
    pinIn(spi0MISO);
    pinMux(spi0MISO);



    TC2Init();
    SPI0init(0);

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


    //buzzing = true;


    volatile long count = 0;
    while (1) {
        ++count;

        if (count > 100) {
            count = 0;
        }

        getAlt();

        pinToggle(LedPin);
        delay_ms(10);

        pinLow(cs_accel);
        spiDataOut(SPI0, 0x80 | (0x32 & 0x3F));
        volatile u8_t ID = spiDataIn(SPI0);
        pinHigh(cs_accel);


        delay_ms(500);



    }
}
