#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {

    my_altimeter->sampleTick = millis();

    my_altimeter->batV = (0.001906 * adc_read(senseBatPin));
    my_altimeter->senseA = adc_read(senseAPin);
    my_altimeter->senseB = adc_read(senseBPin);
    my_altimeter->senseC = adc_read(senseCPin);
    my_altimeter->senseD = adc_read(senseDPin);




    my_altimeter->myAnalogAccelerometer.analogRaw = adc_read(analogAccelPin);
    my_altimeter->myAnalogAccelerometer.analogAccel = (my_altimeter->myAnalogAccelerometer.analogRaw - 3878) * -0.0227;

    my_altimeter->myIMU.perviousAccelX = my_altimeter->myIMU.accelX;



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_baro);
    // dummy_rx = spiDataTransfer(SPI2, 0x50);
    byteOut(spi2SCK,spi2MOSI, 0x50);
    pinHigh(cs_baro);
    delay_us(500);
    my_altimeter->myBarometer.rawTempatureData = readMS5803AdcResults();

    pinLow(cs_baro);
    //  dummy_rx = spiDataTransfer(SPI2, 0x42);
    byteOut(spi2SCK,spi2MOSI, 0x42);
    pinHigh(cs_baro);

// Accel data
    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, 0x80 | 0x02);
    //spiDataOut(SPI0, 0x80 | 0x02);

    my_altimeter->myIMU.accelXLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelXHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelYLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelYHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelZLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelZHigh = spiDataTransfer(SPI0,dummy_Tx);
    //spiDataOut(SPI0, 0x80 | 0x02);
    //my_altimeter->myIMU.accelXLow = spiDataIn(SPI0);
    //my_altimeter->myIMU.accelXHigh = spiDataIn(SPI0);
    //my_altimeter->myIMU.accelYLow = spiDataIn(SPI0);
    //my_altimeter->myIMU.accelYHigh = spiDataIn(SPI0);
    //my_altimeter->myIMU.accelZLow = spiDataIn(SPI0);
    //my_altimeter->myIMU.accelZHigh = spiDataIn(SPI0);
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


    // Gyro data
    pinLow(cs_gyro);
    dummy_rx = spiDataTransfer(SPI0, 0x80 | 0x02);
    my_altimeter->myIMU.gyroXLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroXHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroYLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroYHigh = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroZLow = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroZHigh = spiDataTransfer(SPI0,dummy_Tx);
    pinHigh(cs_gyro);

    bool negativeXgyro = (my_altimeter->myIMU.gyroXHigh & (1<<7)) != 0;
    bool negativeYgyro = (my_altimeter->myIMU.gyroYHigh & (1<<7)) != 0;
    bool negativeZgyro = (my_altimeter->myIMU.gyroZHigh & (1<<7)) != 0;

    my_altimeter->myIMU.gyroXint = (my_altimeter->myIMU.gyroXHigh << 8) | my_altimeter->myIMU.gyroXLow;
    my_altimeter->myIMU.gyroYint = (my_altimeter->myIMU.gyroYHigh << 8) | my_altimeter->myIMU.gyroYLow;
    my_altimeter->myIMU.gyroZint = (my_altimeter->myIMU.gyroZHigh << 8) | my_altimeter->myIMU.gyroZLow;

    if (my_altimeter->myIMU.gyroXHigh != 1) {
        if (negativeXgyro) {
            my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint | ~((1 << 16) - 1);
            my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint >> 4;
        } else {
            my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint >> 4;
        }
    }

    if (my_altimeter->myIMU.gyroYHigh != 1) {
        if (negativeYgyro) {
            my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint | ~((1 << 16) - 1);
            my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint >> 4;
        } else {
            my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint >> 4;
        }
    }

    if (my_altimeter->myIMU.gyroZHigh != 1) {
        if (negativeZgyro) {
            my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint | ~((1 << 16) - 1);
            my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint >> 4;
        } else {
            my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint >> 4;
        }
    }

    my_altimeter->myIMU.gyroX = my_altimeter->myIMU.gyroXint * .00781;
    my_altimeter->myIMU.gyroY = my_altimeter->myIMU.gyroYint * .00781;
    my_altimeter->myIMU.gyroZ = my_altimeter->myIMU.gyroZint * .00781;

    delay_us(800);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();
    ConvertPressureTemperature(&my_altimeter->myBarometer);
    pascalToCent(&my_altimeter->myBarometer);
    my_altimeter->myBarometer.heightFeet = 0.03281 * my_altimeter->myBarometer.heightCm;


}
