#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>

bool takeSample() {
    return SampleBool;
}

void pullSample() {
    SampleBool = true;
}

void sampleTaken() {
    SampleBool = false;
}





void sampleTick(Altimeter *my_altimeter) {
    SampleBool = false;

    // take last samples time to discover Dt before setting new time
    my_altimeter->tickDelta = millis() - my_altimeter->sampleTick;
    my_altimeter->sampleTick = millis();

    my_altimeter->myVoltages.batV = adc_read(senseBatPin);
    my_altimeter->myVoltages.batV = my_altimeter->myVoltages.batV;
    my_altimeter->myVoltages.batFloat = my_altimeter->myVoltages.batV * 0.0019;


    my_altimeter->myVoltages.senseA = adc_read(senseAPin);
    my_altimeter->myVoltages.senseB = adc_read(senseBPin);
    my_altimeter->myVoltages.senseC = adc_read(senseCPin);
    my_altimeter->myVoltages.senseD = adc_read(senseDPin);

    my_altimeter->myAnalogAccelerometer.analogRaw = adc_read(analogAccelPin);
    my_altimeter->myAnalogAccelerometer.analogAccel = (my_altimeter->myAnalogAccelerometer.analogRaw - 3900) * -0.0154;

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
    byteOut(spi2SCK,spi2MOSI, 0x46);
    pinHigh(cs_baro);



// Accel data
    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_ACC_LSB | BMI055_READ_REG);

    my_altimeter->myIMU.accelXRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelXRaw = my_altimeter->myIMU.accelXRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    my_altimeter->myIMU.accelYRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelYRaw = my_altimeter->myIMU.accelYRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    my_altimeter->myIMU.accelZRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.accelZRaw = my_altimeter->myIMU.accelZRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);

    pinHigh(cs_accel);

    bool negativeX = ( my_altimeter->myIMU.accelXRaw & (1<<15)) != 0;
    bool negativeY =  ( my_altimeter->myIMU.accelYRaw & (1<<15)) != 0;
    bool negativeZ =  ( my_altimeter->myIMU.accelZRaw & (1<<15)) != 0;

    my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXRaw;
    my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYRaw;
    my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZRaw;

    if (negativeX) {
        my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint | ~((1 << 16) - 1);
        my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint >> 4;
    } else {
        my_altimeter->myIMU.accelXint = my_altimeter->myIMU.accelXint >> 4;
    }
    if (negativeY) {
        my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint | ~((1 << 16) - 1);
        my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint >> 4;
    } else {
        my_altimeter->myIMU.accelYint = my_altimeter->myIMU.accelYint >> 4;
    }
    if (negativeZ) {
        my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint | ~((1 << 16) - 1);
        my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint >> 4;
    } else {
        my_altimeter->myIMU.accelZint = my_altimeter->myIMU.accelZint >> 4;
    }

    my_altimeter->myIMU.accelX = my_altimeter->myIMU.accelXint * BMI055_ACCEL_16G_DIV;
    my_altimeter->myIMU.accelY = my_altimeter->myIMU.accelYint * BMI055_ACCEL_16G_DIV;
    my_altimeter->myIMU.accelZ = -my_altimeter->myIMU.accelZint * BMI055_ACCEL_16G_DIV;


    // Gyro data
    pinLow(cs_gyro);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_GYRO_LSB | BMI055_READ_REG);
    my_altimeter->myIMU.gyroXRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroXRaw = my_altimeter->myIMU.gyroXRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    my_altimeter->myIMU.gyroYRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroYRaw = my_altimeter->myIMU.gyroYRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    my_altimeter->myIMU.gyroZRaw = spiDataTransfer(SPI0,dummy_Tx);
    my_altimeter->myIMU.gyroZRaw = my_altimeter->myIMU.gyroZRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    pinHigh(cs_gyro);

    bool negativeXgyro = (my_altimeter->myIMU.gyroXRaw & (1<<15)) != 0;
    bool negativeYgyro = (my_altimeter->myIMU.gyroYRaw & (1<<15)) != 0;
    bool negativeZgyro = (my_altimeter->myIMU.gyroZRaw & (1<<15)) != 0;

    my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXRaw;
    my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYRaw;
    my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZRaw;


    if (negativeXgyro) {
        my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint | ~((1 << 16) - 1);
        my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint >> 4;
    } else {
        my_altimeter->myIMU.gyroXint = my_altimeter->myIMU.gyroXint >> 4;
    }

    if (negativeYgyro) {
        my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint | ~((1 << 16) - 1);
        my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint >> 4;
    } else {
        my_altimeter->myIMU.gyroYint = my_altimeter->myIMU.gyroYint >> 4;
    }

    if (negativeZgyro) {
        my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint | ~((1 << 16) - 1);
        my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint >> 4;
    } else {
        my_altimeter->myIMU.gyroZint = my_altimeter->myIMU.gyroZint >> 4;
    }

    my_altimeter->myIMU.gyroX = my_altimeter->myIMU.gyroXint * BMI055_GYRO_2000DS_DIV;
    my_altimeter->myIMU.gyroY = my_altimeter->myIMU.gyroYint * BMI055_GYRO_2000DS_DIV;
    my_altimeter->myIMU.gyroZ = my_altimeter->myIMU.gyroZint * BMI055_GYRO_2000DS_DIV;

    //delay_us(800);
    delay_ms(5);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();
    ConvertPressureTemperature(&my_altimeter->myBarometer);
    paToFeetNOAA(&my_altimeter->myBarometer);


}
