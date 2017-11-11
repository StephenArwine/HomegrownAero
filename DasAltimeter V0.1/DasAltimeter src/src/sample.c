#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {

    my_altimeter->batV = (0.001906 * adc_read(senseBatPin));



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;


    pinLow(cs_baro);
    // dummy_rx = spiDataTransfer(SPI2, 0x50);
    byteOut(spi2SCK,spi2MOSI, 0x50);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawTempatureData = readMS5803AdcResults();

    pinLow(cs_baro);
    //  dummy_rx = spiDataTransfer(SPI2, 0x42);
    byteOut(spi2SCK,spi2MOSI, 0x42);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();


    ConvertPressureTemperature(&my_altimeter->myBarometer);
    pascalToCent(&my_altimeter->myBarometer);
    my_altimeter->myBarometer.heightFeet = 0.03281 * my_altimeter->myBarometer.heightCm;

    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, 0x80 | 0x02);
    my_altimeter->myIMU.accelXLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelXHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelYLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelYHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelZLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelZHigh = spiDataTransfer(SPI0,dummy_Tx);
    pinHigh(cs_accel);

    bool negativeX = (my_altimeter->myIMU.accelXHigh & (1<<7)) != 0;
    bool negativeY = (my_altimeter->myIMU.accelYHigh & (1<<7)) != 0;
    bool negativeZ = (my_altimeter->myIMU.accelZHigh & (1<<7)) != 0;

    my_altimeter->myIMU.accelXint = (my_altimeter->myIMU.accelXHigh << 8) | my_altimeter->myIMU.accelXLow;
    my_altimeter->myIMU.accelYint = (my_altimeter->myIMU.accelYHigh << 8) | my_altimeter->myIMU.accelYLow;
    my_altimeter->myIMU.accelZint = (my_altimeter->myIMU.accelZHigh << 8) | my_altimeter->myIMU.accelZLow;

    if (my_altimeter->myIMU.accelXHigh != 1) {
        if (negativeX) {
            my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint | ~((1 << 16) - 1);
            my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint >> 4;
        } else {
            my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint >> 4;
        }
    }

    if (my_altimeter->myIMU.accelYHigh != 1) {
        if (negativeY) {
            my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint | ~((1 << 16) - 1);
            my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint >> 4;
        } else {
            my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint >> 4;
        }
    }

    if (my_altimeter->myIMU.accelZHigh != 1) {
        if (negativeZ) {
            my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint | ~((1 << 16) - 1);
            my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint >> 4;
        } else {
            my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint >> 4;
        }
    }

    my_altimeter->myIMU.accelX = my_altimeter->myIMU.accelXint * .00781;
    my_altimeter->myIMU.accelY = my_altimeter->myIMU.accelYint * .00781;
    my_altimeter->myIMU.accelZ = my_altimeter->myIMU.accelZint * .00781;


}
