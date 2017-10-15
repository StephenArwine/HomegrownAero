#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {
    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI1, 0x50);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawTempatureData = readMS5803AdcResults();

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI1, 0x42);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();
    flight();

    ConvertPressureTemperature(&my_altimeter->myBarometer);
    pascalToCent(&my_altimeter->myBarometer);
    my_altimeter->myBarometer.heightFeet = 0.03281 * my_altimeter->myBarometer.heightCm;



}