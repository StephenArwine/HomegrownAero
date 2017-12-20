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

volatile sample_t sample;

void sampleTick() {
    SampleBool = false;

    // take last samples time to discover Dt before setting new time
    sample.tickDelta = millis() - sample.sampleTick;
    sample.sampleTick = millis();

    sample.voltage.batV = adc_read(senseBatPin);
    sample.voltage.batFloat = sample.voltage.batV * 0.0019;

    sample.voltage.senseA = adc_read(senseAPin);
    sample.voltage.senseB = adc_read(senseBPin);
    sample.voltage.senseC = adc_read(senseCPin);
    sample.voltage.senseD = adc_read(senseDPin);

    sample.analogRaw = adc_read(analogAccelPin);
    sample.analogAccel = (sample.analogRaw - 3900) * -0.0154;



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_baro);
    // dummy_rx = spiDataTransfer(SPI2, 0x50);
    byteOut(spi2SCK,spi2MOSI, 0x50);
    pinHigh(cs_baro);
    delay_us(500);
    uint32_t tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    //  dummy_rx = spiDataTransfer(SPI2, 0x42);
    byteOut(spi2SCK,spi2MOSI, 0x46);
    pinHigh(cs_baro);



// Accel data
    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_ACC_LSB | BMI055_READ_REG);

    u16_t accelXRaw = spiDataTransfer(SPI0,dummy_Tx);
    accelXRaw = accelXRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    u16_t accelYRaw = spiDataTransfer(SPI0,dummy_Tx);
    accelYRaw = accelYRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    u16_t accelZRaw = spiDataTransfer(SPI0,dummy_Tx);
    accelZRaw = accelZRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);

    pinHigh(cs_accel);

    bool negativeX = ( accelXRaw & (1<<15)) != 0;
    bool negativeY =  ( accelYRaw & (1<<15)) != 0;
    bool negativeZ =  ( accelZRaw & (1<<15)) != 0;

    int16_t accelXint = accelXRaw;
    int16_t accelYint = accelYRaw;
    int16_t accelZint = accelZRaw;

    if (negativeX) {
        accelXint = accelXint | ~((1 << 16) - 1);
        accelXint =  accelXint >> 4;
    } else {
        accelXint =  accelXint >> 4;
    }
    if (negativeY) {
        accelYint =  accelYint | ~((1 << 16) - 1);
        accelYint =  accelYint >> 4;
    } else {
        accelYint =  accelYint >> 4;
    }
    if (negativeZ) {
        accelZint =  accelZint | ~((1 << 16) - 1);
        accelZint =  accelZint >> 4;
    } else {
        accelZint =  accelZint >> 4;
    }

    sample.accelX =  accelXint * BMI055_ACCEL_16G_DIV;
    sample.accelY =  accelYint * BMI055_ACCEL_16G_DIV;
    sample.accelZ = -accelZint * BMI055_ACCEL_16G_DIV;


    // Gyro data
    pinLow(cs_gyro);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_GYRO_LSB | BMI055_READ_REG);
    u16_t gyroXRaw = spiDataTransfer(SPI0,dummy_Tx);
    gyroXRaw =  gyroXRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    u16_t gyroYRaw = spiDataTransfer(SPI0,dummy_Tx);
    gyroYRaw =  gyroYRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    u16_t gyroZRaw = spiDataTransfer(SPI0,dummy_Tx);
    gyroZRaw =  gyroZRaw |  (spiDataTransfer(SPI0,dummy_Tx) << 8);
    pinHigh(cs_gyro);

    bool negativeXgyro = ( gyroXRaw & (1<<15)) != 0;
    bool negativeYgyro = ( gyroYRaw & (1<<15)) != 0;
    bool negativeZgyro = ( gyroZRaw & (1<<15)) != 0;

    int16_t gyroXint =  gyroXRaw;
    int16_t gyroYint =  gyroYRaw;
    int16_t gyroZint =  gyroZRaw;


    if (negativeXgyro) {
        gyroXint =  gyroXint | ~((1 << 16) - 1);
        gyroXint =  gyroXint >> 4;
    } else {
        gyroXint =  gyroXint >> 4;
    }

    if (negativeYgyro) {
        gyroYint =  gyroYint | ~((1 << 16) - 1);
        gyroYint =  gyroYint >> 4;
    } else {
        gyroYint =  gyroYint >> 4;
    }

    if (negativeZgyro) {
        gyroZint =  gyroZint | ~((1 << 16) - 1);
        gyroZint =  gyroZint >> 4;
    } else {
        gyroZint =  gyroZint >> 4;
    }

    sample.gyroX =  gyroXint * BMI055_GYRO_2000DS_DIV;
    sample.gyroY =  gyroYint * BMI055_GYRO_2000DS_DIV;
    sample.gyroZ =  gyroZint * BMI055_GYRO_2000DS_DIV;

    //delay_us(800);
    delay_ms(5);
    u32_t PressureRaw = readMS5803AdcResults();
    ConvertPressureTemperature(PressureRaw, tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);
    sample.altitudefeet = paToFeetNOAA(sample.pressureMbar);


}
